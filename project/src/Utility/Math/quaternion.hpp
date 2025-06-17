#pragma once
#include "DxLib.h"
#include <vector>
#include <memory>
#include <cmath>
#include "Utility/Math/math.hpp"

namespace my_math
{
    class Quaternion final
    {
    public:
        float x_, y_, z_, w_;

        explicit Quaternion()
            : x_(0.0f), y_(0.0f), z_(0.0f), w_(1.0f)
        {
        }

        explicit Quaternion(float x, float y, float z, float w)
            : x_(x), y_(y), z_(z), w_(w)
        {
        }

        static Quaternion Identity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }

        // オイラー角から四元数を作成（度数法）
        static Quaternion FromEulerDegrees(float pitch, float yaw, float roll)
        {
            return FromEulerRadians(
                pitch * DX_PI_F / 180.0f,
                yaw   * DX_PI_F / 180.0f,
                roll  * DX_PI_F / 180.0f
            );
        }

        // オイラー角から四元数を作成（ラジアン）
        static Quaternion FromEulerRadians(float pitch, float yaw, float roll)
        {
            float cy = cos(yaw   * 0.5f);
            float sy = sin(yaw   * 0.5f);
            float cp = cos(pitch * 0.5f);
            float sp = sin(pitch * 0.5f);
            float cr = cos(roll  * 0.5f);
            float sr = sin(roll  * 0.5f);

            return Quaternion(
                sp * cy * cr - cp * sy * sr,  // x_
                cp * sy * cr + sp * cy * sr,  // y_
                cp * cy * sr - sp * sy * cr,  // z_
                cp * cy * cr + sp * sy * sr   // w_
            );
        }

        // 軸と角度から四元数を作成
        static Quaternion FromAxisAngle(const VECTOR& axis, float angle)
        {
            VECTOR normalized_axis = VNorm(axis);
            float  half_angle      = angle * 0.5f;
            float  sin_half_angle  = sin(half_angle);

            return Quaternion(
                normalized_axis.x * sin_half_angle,
                normalized_axis.y * sin_half_angle,
                normalized_axis.z * sin_half_angle,
                cos(half_angle)
            );
        }

        // 2つのベクトル間の回転を作成
        static Quaternion FromToRotation(const VECTOR& from, const VECTOR& to)
        {
            VECTOR from_norm = VNorm(from);
            VECTOR to_norm   = VNorm(to);

            /// 2つのベクトルの内積
            float dot = VDot(from_norm, to_norm);

            if (dot >= 0.999999f) {
                return Identity();
            }

            if (dot <= -0.999999f) {
                VECTOR axis = VCross(VGet(1.0f, 0.0f, 0.0f), from_norm);

                if (VSquareSize(axis) < 0.000001f) {
                    axis = VCross(VGet(0.0f, 1.0f, 0.0f), from_norm);
                }

                axis = VNorm(axis);
                return FromAxisAngle(axis, DX_PI_F);
            }

            VECTOR axis = VCross(from_norm, to_norm);
            float  w    = sqrt(VSquareSize(from_norm) * VSquareSize(to_norm)) + dot;

            Quaternion result(axis.x, axis.y, axis.z, w);
            return result.Normalized();
        }

        // 正規化
        Quaternion Normalized() const
        {
            float len = sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
            if (len < 0.000001f) {
                return Identity();
            }

            float inv_len = 1.0f / len;
            return Quaternion(x_ * inv_len, y_ * inv_len, z_ * inv_len, w_ * inv_len);
        }

        // 正規化（自身を変更）
        void Normalize()
        {
            *this = Normalized();
        }

        // 共役に変換
        Quaternion Conjugate() const
        {
            return Quaternion(-x_, -y_, -z_, w_);
        }

        // 逆元
        Quaternion Inverse() const
        {
            float length_sq = x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_;
            if (length_sq < 0.000001f) {
                return Identity();
            }

            Quaternion conj     = Conjugate();
            float inv_length_sq = 1.0f / length_sq;

            return Quaternion(
                conj.x_ * inv_length_sq,
                conj.y_ * inv_length_sq,
                conj.z_ * inv_length_sq,
                conj.w_ * inv_length_sq
            );
        }

        // 四元数の乗算
        Quaternion operator*(const Quaternion& other) const
        {
            return Quaternion(
                w_ * other.x_ + x_ * other.w_ + y_ * other.z_ - z_ * other.y_,
                w_ * other.y_ - x_ * other.z_ + y_ * other.w_ + z_ * other.x_,
                w_ * other.z_ + x_ * other.y_ - y_ * other.x_ + z_ * other.w_,
                w_ * other.w_ - x_ * other.x_ - y_ * other.y_ - z_ * other.z_
            );
        }

        // ベクトルの回転
        VECTOR RotateVector(const VECTOR& vec) const
        {
            Quaternion vec_quat(vec.x, vec.y, vec.z, 0.0f);
            Quaternion result = (*this) * vec_quat * Conjugate();
            return VGet(result.x_, result.y_, result.z_);
        }

        // 回転行列に変換
        MATRIX ToMatrix() const
        {
            float xx = x_ * x_;
            float yy = y_ * y_;
            float zz = z_ * z_;
            float xy = x_ * y_;
            float xz = x_ * z_;
            float yz = y_ * z_;
            float wx = w_ * x_;
            float wy = w_ * y_;
            float wz = w_ * z_;

            MATRIX matrix = MGetIdent();
            matrix.m[0][0] = 1.0f - 2.0f * (yy + zz);
            matrix.m[0][1] = 2.0f * (xy - wz);
            matrix.m[0][2] = 2.0f * (xz + wy);
            matrix.m[0][3] = 0.0f;

            matrix.m[1][0] = 2.0f * (xy + wz);
            matrix.m[1][1] = 1.0f - 2.0f * (xx + zz);
            matrix.m[1][2] = 2.0f * (yz - wx);
            matrix.m[1][3] = 0.0f;

            matrix.m[2][0] = 2.0f * (xz - wy);
            matrix.m[2][1] = 2.0f * (yz + wx);
            matrix.m[2][2] = 1.0f - 2.0f * (xx + yy);
            matrix.m[2][3] = 0.0f;

            matrix.m[3][0] = 0.0f;
            matrix.m[3][1] = 0.0f;
            matrix.m[3][2] = 0.0f;
            matrix.m[3][3] = 1.0f;

            return matrix;
        }

        // オイラー角に変換（ラジアン）
        VECTOR ToEulerRadians() const
        {
            VECTOR result = VGet(0.0f, 0.0f, 0.0f);

            // Roll (X軸回転)
            float sinr_cosp = 2.0f        * (w_ * x_ + y_ * z_);
            float cosr_cosp = 1.0f - 2.0f * (x_ * x_ + y_ * y_);
            result.z        = atan2(sinr_cosp, cosr_cosp);

            // Pitch (Y軸回転)
            float sinp = 2.0f * (w_ * y_ - z_ * x_);
            if (fabs(sinp) >= 1.0f) {
                result.x = copysign(DX_PI_F / 2.0f, sinp);
            }
            else {
                result.x = asin(sinp);
            }

            // Yaw (Z軸回転)
            float siny_cosp = 2.0f        * (w_ * z_ + x_ * y_);
            float cosy_cosp = 1.0f - 2.0f * (y_ * y_ + z_ * z_);
            result.y        = atan2(siny_cosp, cosy_cosp);

            return result;
        }

        // 球面線形補間
        static Quaternion Slerp(const Quaternion& from, const Quaternion& to, float t)
        {
            Quaternion q1 = from.Normalized();
            Quaternion q2 = to.Normalized();

            float dot = (q1.x_ * q2.x_) + (q1.y_ * q2.y_) + (q1.z_ * q2.z_) + (q1.w_ + q2.w_);

            if (dot < 0.0f) {
                q2 = Quaternion(-q2.x_, -q2.y_, -q2.z_, -q2.w_);
                dot = -dot;
            }

            if (dot > 0.9995f) {
                // 線形補間
                return Quaternion(
                    q1.x_ + t * (q2.x_ - q1.x_),
                    q1.y_ + t * (q2.y_ - q1.y_),
                    q1.z_ + t * (q2.z_ - q1.z_),
                    q1.w_ + t * (q2.w_ - q1.w_)
                ).Normalized();
            }

            float theta     = acos(dot);
            float sin_theta = sin(theta);
            float weight1   = sin((1.0f - t) * theta) / sin_theta;
            float weight2   = sin(t * theta)          / sin_theta;

            return Quaternion(
                weight1 * q1.x_ + weight2 * q2.x_,
                weight1 * q1.y_ + weight2 * q2.y_,
                weight1 * q1.z_ + weight2 * q2.z_,
                weight1 * q1.w_ + weight2 * q2.w_
            );
        }
    };

    /// @brief 2つの回転の内積
    /// @brief 内積（cosθ）
    /*
    [[nodiscard]] const float& QDot(Quaternion q1, Quaternion q2)
    {
        return (q1.x_ * q2.x_) + (q1.y_ * q2.y_) + (q1.z_ * q2.z_) + (q1.w_ + q2.w_);
    };
    */

} // my_math end.