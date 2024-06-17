// *********************************************************************************
// DirectXMathの計算拡張機能をもつ
// Y.S_AT12D33 reference...
// *********************************************************************************
#pragma once
//----------------------------------------------------------------------------------
// ヘッダーファイルの取得
#include <DirectXMath.h>
#include <cmath>
#include <algorithm>
#include <numeric>

inline namespace XmfloatMath
{
	// クランプ---------------------------------------------------------------------------------------------------
	_NODISCARD static inline auto Clamp(const DirectX::XMFLOAT2& vf2, const DirectX::XMFLOAT2& low, const DirectX::XMFLOAT2& high)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = std::clamp(vf2.x, low.x, high.x);
		temp.y = std::clamp(vf2.y, low.y, high.y);

		return temp;
	}

	_NODISCARD static inline auto Clamp(const DirectX::XMFLOAT3& vf3, const DirectX::XMFLOAT3& low, const DirectX::XMFLOAT3& high)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = std::clamp(vf3.x, low.x, high.x);
		temp.y = std::clamp(vf3.y, low.y, high.y);
		temp.z = std::clamp(vf3.z, low.z, high.z);

		return temp;
	}

	_NODISCARD static inline auto Clamp(const DirectX::XMFLOAT4& vf4, const DirectX::XMFLOAT4& low, const DirectX::XMFLOAT4& high)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = std::clamp(vf4.x, low.x, high.x);
		temp.y = std::clamp(vf4.y, low.y, high.y);
		temp.z = std::clamp(vf4.z, low.z, high.z);
		temp.w = std::clamp(vf4.w, low.w, high.w);

		return temp;
	}

	_NODISCARD static inline auto Clamp(const DirectX::XMFLOAT2& vf2, const float low, const float high)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = std::clamp(vf2.x, low, high);
		temp.y = std::clamp(vf2.y, low, high);

		return temp;
	}

	_NODISCARD static inline auto Clamp(const DirectX::XMFLOAT3& vf3, const float low, const float high)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = std::clamp(vf3.x, low, high);
		temp.y = std::clamp(vf3.y, low, high);
		temp.z = std::clamp(vf3.z, low, high);

		return temp;
	}

	_NODISCARD static inline auto Clamp(const DirectX::XMFLOAT4& vf4, const float low, const float high)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = std::clamp(vf4.x, low, high);
		temp.y = std::clamp(vf4.y, low, high);
		temp.z = std::clamp(vf4.z, low, high);
		temp.w = std::clamp(vf4.w, low, high);

		return temp;
	}

	// マックス---------------------------------------------------------------------------------------------------
	_NODISCARD static inline auto Max(const DirectX::XMFLOAT2& left, const DirectX::XMFLOAT2& right)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = (std::max)(left.x, right.x);
		temp.y = (std::max)(left.y, right.y);

		return temp;
	}

	_NODISCARD static inline auto Max(const DirectX::XMFLOAT3& left, const DirectX::XMFLOAT3& right)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = (std::max)(left.x, right.x);
		temp.y = (std::max)(left.y, right.y);
		temp.z = (std::max)(left.z, right.z);

		return temp;
	}

	_NODISCARD static inline auto Max(const DirectX::XMFLOAT4& left, const DirectX::XMFLOAT4& right)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = (std::max)(left.x, right.x);
		temp.y = (std::max)(left.y, right.y);
		temp.z = (std::max)(left.z, right.z);
		temp.w = (std::max)(left.w, right.w);

		return temp;
	}

	_NODISCARD static inline auto Max(const DirectX::XMFLOAT2& left, const float right)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = (std::max)(left.x, right);
		temp.y = (std::max)(left.y, right);

		return temp;
	}

	_NODISCARD static inline auto Max(const DirectX::XMFLOAT3& left, const float right)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = (std::max)(left.x, right);
		temp.y = (std::max)(left.y, right);
		temp.z = (std::max)(left.z, right);

		return temp;
	}

	_NODISCARD static inline auto Max(const DirectX::XMFLOAT4& left, const float right)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = (std::max)(left.x, right);
		temp.y = (std::max)(left.y, right);
		temp.z = (std::max)(left.z, right);
		temp.w = (std::max)(left.w, right);

		return temp;
	}

	// ミニ---------------------------------------------------------------------------------------------------
	_NODISCARD static inline auto Min(const DirectX::XMFLOAT2& left, const DirectX::XMFLOAT2& right)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = (std::min)(left.x, right.x);
		temp.y = (std::min)(left.y, right.y);

		return temp;
	}

	_NODISCARD static inline auto Min(const DirectX::XMFLOAT3& left, const DirectX::XMFLOAT3& right)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = (std::min)(left.x, right.x);
		temp.y = (std::min)(left.y, right.y);
		temp.z = (std::min)(left.z, right.z);

		return temp;
	}

	_NODISCARD static inline auto Min(const DirectX::XMFLOAT4& left, const DirectX::XMFLOAT4& right)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = (std::min)(left.x, right.x);
		temp.y = (std::min)(left.y, right.y);
		temp.z = (std::min)(left.z, right.z);
		temp.w = (std::min)(left.w, right.w);

		return temp;
	}

	_NODISCARD static inline auto Min(const DirectX::XMFLOAT2& left, const float right)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = (std::min)(left.x, right);
		temp.y = (std::min)(left.y, right);

		return temp;
	}

	_NODISCARD static inline auto Min(const DirectX::XMFLOAT3& left, const float right)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = (std::min)(left.x, right);
		temp.y = (std::min)(left.y, right);
		temp.z = (std::min)(left.z, right);

		return temp;
	}

	_NODISCARD static inline auto Min(const DirectX::XMFLOAT4& left, const float right)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = (std::min)(left.x, right);
		temp.y = (std::min)(left.y, right);
		temp.z = (std::min)(left.z, right);
		temp.w = (std::min)(left.w, right);

		return temp;
	}

	// 2 を底とする指数関数---------------------------------------------------------------------------------------------------
	// 2 の 引数 乗を返す。

	_NODISCARD static inline auto Exp2(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = exp2f(vf2.x);
		temp.y = exp2f(vf2.y);

		return temp;
	}

	_NODISCARD static inline auto Exp2(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = exp2f(vf3.x);
		temp.y = exp2f(vf3.y);
		temp.z = exp2f(vf3.z);

		return temp;
	}

	_NODISCARD static inline auto Exp2(const DirectX::XMFLOAT4& vf4)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = exp2f(vf4.x);
		temp.y = exp2f(vf4.y);
		temp.z = exp2f(vf4.z);
		temp.w = exp2f(vf4.w);

		return temp;
	}

	// 2 を底とする二進対数---------------------------------------------------------------------------------------------------
	// 引数 の 2 を底とする二進対数を返す。（引数が２の何乗かを返す）

	_NODISCARD static inline auto Log2(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = log2f(vf2.x);
		temp.y = log2f(vf2.y);

		return temp;
	}

	_NODISCARD static inline auto Log2(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = log2f(vf3.x);
		temp.y = log2f(vf3.y);
		temp.z = log2f(vf3.z);

		return temp;
	}

	_NODISCARD static inline auto Log2(const DirectX::XMFLOAT4& vf4)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = log2f(vf4.x);
		temp.y = log2f(vf4.y);
		temp.z = log2f(vf4.z);
		temp.w = log2f(vf4.w);

		return temp;
	}

	// 2 を底とする二進対数---------------------------------------------------------------------------------------------------
	// 引数 x の 10 を底とする常用対数を返す。(引数が10の何乗かを返す)

	_NODISCARD static inline auto Log10(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = log10f(vf2.x);
		temp.y = log10f(vf2.y);

		return temp;
	}

	_NODISCARD static inline auto Log10(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = log10f(vf3.x);
		temp.y = log10f(vf3.y);
		temp.z = log10f(vf3.z);

		return temp;
	}

	_NODISCARD static inline auto Log10(const DirectX::XMFLOAT4& vf4)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = log10f(vf4.x);
		temp.y = log10f(vf4.y);
		temp.z = log10f(vf4.z);
		temp.w = log10f(vf4.w);

		return temp;
	}

	// 切り上げ---------------------------------------------------------------------------------------------------

	_NODISCARD static inline auto Ceil(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = ceilf(vf2.x);
		temp.y = ceilf(vf2.y);

		return temp;
	}

	_NODISCARD static inline auto Ceil(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = ceilf(vf3.x);
		temp.y = ceilf(vf3.y);
		temp.z = ceilf(vf3.z);

		return temp;
	}

	_NODISCARD static inline auto Ceil(const DirectX::XMFLOAT4& vf4)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = ceilf(vf4.x);
		temp.y = ceilf(vf4.y);
		temp.z = ceilf(vf4.z);
		temp.w = ceilf(vf4.w);

		return temp;
	}

	// 切り捨て---------------------------------------------------------------------------------------------------

	_NODISCARD static inline auto Floor(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = floorf(vf2.x);
		temp.y = floorf(vf2.y);

		return temp;
	}

	_NODISCARD static inline auto Floor(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = floorf(vf3.x);
		temp.y = floorf(vf3.y);
		temp.z = floorf(vf3.z);

		return temp;
	}

	_NODISCARD static inline auto Floor(const DirectX::XMFLOAT4& vf4)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = floorf(vf4.x);
		temp.y = floorf(vf4.y);
		temp.z = floorf(vf4.z);
		temp.w = floorf(vf4.w);

		return temp;
	}

	// ゼロ方向への丸め---------------------------------------------------------------------------------------------------

	_NODISCARD static inline auto Trunc(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = truncf(vf2.x);
		temp.y = truncf(vf2.y);

		return temp;
	}

	_NODISCARD static inline auto Trunc(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = truncf(vf3.x);
		temp.y = truncf(vf3.y);
		temp.z = truncf(vf3.z);

		return temp;
	}

	_NODISCARD static inline auto Trunc(const DirectX::XMFLOAT4& vf4)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = truncf(vf4.x);
		temp.y = truncf(vf4.y);
		temp.z = truncf(vf4.z);
		temp.w = truncf(vf4.w);

		return temp;
	}

	// 四捨五入---------------------------------------------------------------------------------------------------

	_NODISCARD static inline auto Round(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = roundf(vf2.x);
		temp.y = roundf(vf2.y);

		return temp;
	}

	_NODISCARD static inline auto Round(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = roundf(vf3.x);
		temp.y = roundf(vf3.y);
		temp.z = roundf(vf3.z);

		return temp;
	}

	_NODISCARD static inline auto Round(const DirectX::XMFLOAT4& vf4)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = roundf(vf4.x);
		temp.y = roundf(vf4.y);
		temp.z = roundf(vf4.z);
		temp.w = roundf(vf4.w);

		return temp;
	}

	//. XMFFLOATの全ての変数に同じ値を代入-------------------------------------------------------------------------

	_NODISCARD static constexpr inline auto Fill2(const float num)
	{
		DirectX::XMFLOAT2 temp{ num, num };

		return temp;
	}

	_NODISCARD static constexpr inline auto Fill3(const float num)
	{
		DirectX::XMFLOAT3 temp{ num, num, num };

		return temp;
	}

	_NODISCARD static constexpr inline auto Fill4(const float num)
	{
		DirectX::XMFLOAT4 temp{ num, num, num, num };

		return temp;
	}

	// 累乗---------------------------------------------------------------------------------------------------

	_NODISCARD static inline auto Pow(const DirectX::XMFLOAT2& vf2, const float pow_num)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = powf(vf2.x, pow_num);
		temp.y = powf(vf2.y, pow_num);

		return temp;
	}

	_NODISCARD static inline auto Pow(const DirectX::XMFLOAT3& vf3, const float pow_num)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = powf(vf3.x, pow_num);
		temp.y = powf(vf3.y, pow_num);
		temp.z = powf(vf3.z, pow_num);

		return temp;
	}

	_NODISCARD static inline auto Pow(const DirectX::XMFLOAT4& vf4, const float pow_num)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = powf(vf4.x, pow_num);
		temp.y = powf(vf4.y, pow_num);
		temp.z = powf(vf4.z, pow_num);
		temp.w = powf(vf4.w, pow_num);

		return temp;
	}

	// 平方根---------------------------------------------------------------------------------------------------

	_NODISCARD static inline auto Sqrt(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = sqrtf(vf2.x);
		temp.y = sqrtf(vf2.y);

		return temp;
	}

	_NODISCARD static inline auto Sqrt(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = sqrtf(vf3.x);
		temp.y = sqrtf(vf3.y);
		temp.z = sqrtf(vf3.z);

		return temp;
	}

	_NODISCARD static inline auto Sqrt(const DirectX::XMFLOAT4& vf4)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = sqrtf(vf4.x);
		temp.y = sqrtf(vf4.y);
		temp.z = sqrtf(vf4.z);
		temp.w = sqrtf(vf4.w);

		return temp;
	}

	// 立方根---------------------------------------------------------------------------------------------------

	_NODISCARD static inline auto Cbrt(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = cbrtf(vf2.x);
		temp.y = cbrtf(vf2.y);

		return temp;
	}

	_NODISCARD static inline auto Cbrt(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = cbrtf(vf3.x);
		temp.y = cbrtf(vf3.y);
		temp.z = cbrtf(vf3.z);

		return temp;
	}

	_NODISCARD static inline auto Cbrt(const DirectX::XMFLOAT4& vf4)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = cbrtf(vf4.x);
		temp.y = cbrtf(vf4.y);
		temp.z = cbrtf(vf4.z);
		temp.w = cbrtf(vf4.w);

		return temp;
	}

	// 絶対値---------------------------------------------------------------------------------------------------

	_NODISCARD static inline auto FAbs(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 temp{};

		temp.x = fabs(vf2.x);
		temp.y = fabs(vf2.y);

		return temp;
	}

	_NODISCARD static inline auto FAbs(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 temp{};

		temp.x = fabs(vf3.x);
		temp.y = fabs(vf3.y);
		temp.z = fabs(vf3.z);

		return temp;
	}

	_NODISCARD static inline auto FAbs(const DirectX::XMFLOAT4& vf4)
	{
		DirectX::XMFLOAT4 temp{};

		temp.x = fabs(vf4.x);
		temp.y = fabs(vf4.y);
		temp.z = fabs(vf4.z);
		temp.w = fabs(vf4.w);

		return temp;
	}

	// ０クリアー--------------------------------------------------------------------------------------------------

	static inline void Clear(DirectX::XMFLOAT2& vf2)
	{
		vf2 = { 0.f, 0.f };
	}

	static inline void Clear(DirectX::XMFLOAT3& vf3)
	{
		vf3 = { 0.f, 0.f, 0.f };
	}

	static inline void Clear(DirectX::XMFLOAT4& vf4)
	{
		vf4 = { 0.f, 0.f, 0.f, 0.f };
	}

	// 正規化-----------------------------------------------------------------------------------------------------

	_NODISCARD static inline auto VectorNormalize(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 rv{ vf2 };

		auto&& vec{ DirectX::XMLoadFloat2(&rv) };

		DirectX::XMStoreFloat2(&rv, DirectX::XMVector2Normalize(vec));

		return rv;
	}

	_NODISCARD static inline auto VectorNormalize(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 rv{ vf3 };

		auto&& vec{ DirectX::XMLoadFloat3(&rv) };

		DirectX::XMStoreFloat3(&rv, DirectX::XMVector3Normalize(vec));

		return rv;
	}

	_NODISCARD static inline auto VectorNormalize(const DirectX::XMFLOAT4& vf4)
	{
		DirectX::XMFLOAT4 rv{ vf4 };

		auto&& vec{ DirectX::XMLoadFloat4(&rv) };

		DirectX::XMStoreFloat4(&rv, DirectX::XMVector4Normalize(vec));

		return rv;
	}

	// ベクトルの長さを取得----------------------------------------------------------------------------------------------------

	_NODISCARD static inline float VectorLength(const DirectX::XMFLOAT2& vf2)
	{
		float len{};

		auto&& vec{ DirectX::XMLoadFloat2(&vf2) };

		DirectX::XMStoreFloat(&len, DirectX::XMVector2Length(vec));

		return len;
	}

	_NODISCARD static inline float VectorLength(const DirectX::XMFLOAT3& vf3)
	{
		float len{};

		auto&& vec{ DirectX::XMLoadFloat3(&vf3) };

		DirectX::XMStoreFloat(&len, DirectX::XMVector3Length(vec));

		return len;
	}

	_NODISCARD static inline float VectorLength(const DirectX::XMFLOAT4& vf4)
	{
		float len{};

		auto&& vec{ DirectX::XMLoadFloat4(&vf4) };

		DirectX::XMStoreFloat(&len, DirectX::XMVector4Length(vec));

		return len;
	}

	// ベクトルの長さの二乗を取得----------------------------------------------------------------------------------------------------

	_NODISCARD static inline float VectorLengthSq(const DirectX::XMFLOAT2& vf2)
	{
		float len{};

		auto&& vec{ DirectX::XMLoadFloat2(&vf2) };

		DirectX::XMStoreFloat(&len, DirectX::XMVector2LengthSq(vec));

		return len;
	}

	_NODISCARD static inline float VectorLengthSq(const DirectX::XMFLOAT3& vf3)
	{
		float len{};

		auto&& vec{ DirectX::XMLoadFloat3(&vf3) };

		DirectX::XMStoreFloat(&len, DirectX::XMVector3LengthSq(vec));

		return len;
	}

	_NODISCARD static inline float VectorLengthSq(const DirectX::XMFLOAT4& vf4)
	{
		float len{};

		auto&& vec{ DirectX::XMLoadFloat4(&vf4) };

		DirectX::XMStoreFloat(&len, DirectX::XMVector4LengthSq(vec));

		return len;
	}

	// ベクトルの直行するベクトルを計算----------------------------------------------------------------------------------------------------

	_NODISCARD static inline auto VectorOrthogonal(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 rv{};

		auto&& vec{ DirectX::XMLoadFloat2(&vf2) };

		DirectX::XMStoreFloat2(&rv, DirectX::XMVector2Orthogonal(vec));

		return rv;
	}

	_NODISCARD static inline auto VectorOrthogonal(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 rv{};

		auto&& vec{ DirectX::XMLoadFloat3(&vf3) };

		DirectX::XMStoreFloat3(&rv, DirectX::XMVector3Orthogonal(vec));

		return rv;
	}

	_NODISCARD static inline auto VectorOrthogonal(const DirectX::XMFLOAT4& vf4)
	{
		DirectX::XMFLOAT4 rv{};

		auto&& vec{ DirectX::XMLoadFloat4(&vf4) };

		DirectX::XMStoreFloat4(&rv, DirectX::XMVector4Orthogonal(vec));

		return rv;
	}

	// ベクトルの内積を計算----------------------------------------------------------------------------------------------------

	_NODISCARD static inline auto VectorDot(const DirectX::XMFLOAT2& vec1, const DirectX::XMFLOAT2& vec2)
	{
		float rv{};

		const auto&& v1{ DirectX::XMLoadFloat2(&vec1) }, &&v2{ DirectX::XMLoadFloat2(&vec2) };

		DirectX::XMStoreFloat(&rv, DirectX::XMVector2Dot(v1, v2));

		return rv;
	}

	_NODISCARD static inline auto VectorDot(const DirectX::XMFLOAT3& vec1, const DirectX::XMFLOAT3& vec2)
	{
		float rv{};

		const auto&& v1{ DirectX::XMLoadFloat3(&vec1) }, &&v2{ DirectX::XMLoadFloat3(&vec2) };

		DirectX::XMStoreFloat(&rv, DirectX::XMVector3Dot(v1, v2));

		return rv;
	}

	_NODISCARD static inline auto VectorDot(const DirectX::XMFLOAT4& vec1, const DirectX::XMFLOAT4& vec2)
	{
		float rv{};

		const auto&& v1{ DirectX::XMLoadFloat4(&vec1) }, &&v2{ DirectX::XMLoadFloat4(&vec2) };

		DirectX::XMStoreFloat(&rv, DirectX::XMVector4Dot(v1, v2));

		return rv;
	}

	// ベクトルの外積を計算----------------------------------------------------------------------------------------------------

	_NODISCARD static inline auto VectorCross(const DirectX::XMFLOAT2& vec1, const DirectX::XMFLOAT2& vec2)
	{
		float rv{};

		const auto&& v1{ DirectX::XMLoadFloat2(&vec1) }, &&v2{ DirectX::XMLoadFloat2(&vec2) };

		DirectX::XMStoreFloat(&rv, DirectX::XMVector2Cross(v1, v2));

		return rv;
	}

	_NODISCARD static inline auto VectorCross(const DirectX::XMFLOAT3& vec1, const DirectX::XMFLOAT3& vec2)
	{
		DirectX::XMFLOAT3 rv{};

		const auto&& v1{ DirectX::XMLoadFloat3(&vec1) }, &&v2{ DirectX::XMLoadFloat3(&vec2) };

		DirectX::XMStoreFloat3(&rv, DirectX::XMVector3Cross(v1, v2));

		return rv;
	}

	// ベクトル間のラジアン角度を計算-----------------------------------------------------------------------------------------

	_NODISCARD static inline float AngleBetweenVectors(const DirectX::XMFLOAT2& vf2_left, const DirectX::XMFLOAT2& vf2_right)
	{
		float rad{};

		auto&& l_vec{ DirectX::XMLoadFloat2(&vf2_left) }, && r_vec{ DirectX::XMLoadFloat2(&vf2_right) };

		DirectX::XMStoreFloat(&rad, DirectX::XMVector2AngleBetweenVectors(l_vec, r_vec));

		return rad;
	}

	_NODISCARD static inline float AngleBetweenVectors(const DirectX::XMFLOAT3& vf3_left, const DirectX::XMFLOAT3& vf3_right)
	{
		float rad{};

		auto&& l_vec{ DirectX::XMLoadFloat3(&vf3_left) }, && r_vec{ DirectX::XMLoadFloat3(&vf3_right) };

		DirectX::XMStoreFloat(&rad, DirectX::XMVector3AngleBetweenVectors(l_vec, r_vec));

		return rad;
	}

	_NODISCARD static inline float AngleBetweenVectors(const DirectX::XMFLOAT4& vf4_left, const DirectX::XMFLOAT4& vf4_right)
	{
		float rad{};

		auto&& l_vec{ DirectX::XMLoadFloat4(&vf4_left) }, && r_vec{ DirectX::XMLoadFloat4(&vf4_right) };

		DirectX::XMStoreFloat(&rad, DirectX::XMVector4AngleBetweenVectors(l_vec, r_vec));

		return rad;
	}

	// ベクトル間のラジアン角度を計算-----------------------------------------------------------------------------------------

	_NODISCARD static inline float AngleBetweenNormals(const DirectX::XMFLOAT2& vf2_left, const DirectX::XMFLOAT2& vf2_right)
	{
		float rad{};

		auto&& l_vec{ DirectX::XMLoadFloat2(&vf2_left) }, && r_vec{ DirectX::XMLoadFloat2(&vf2_right) };

		DirectX::XMStoreFloat(&rad, DirectX::XMVector2AngleBetweenNormals(l_vec, r_vec));

		return rad;
	}

	_NODISCARD static inline float AngleBetweenNormals(const DirectX::XMFLOAT3& vf3_left, const DirectX::XMFLOAT3& vf3_right)
	{
		float rad{};

		auto&& l_vec{ DirectX::XMLoadFloat3(&vf3_left) }, && r_vec{ DirectX::XMLoadFloat3(&vf3_right) };

		DirectX::XMStoreFloat(&rad, DirectX::XMVector3AngleBetweenNormals(l_vec, r_vec));

		return rad;
	}

	_NODISCARD static inline float AngleBetweenNormals(const DirectX::XMFLOAT4& vf4_left, const DirectX::XMFLOAT4& vf4_right)
	{
		float rad{};

		auto&& l_vec{ DirectX::XMLoadFloat4(&vf4_left) }, && r_vec{ DirectX::XMLoadFloat4(&vf4_right) };

		DirectX::XMStoreFloat(&rad, DirectX::XMVector4AngleBetweenNormals(l_vec, r_vec));

		return rad;
	}

	// 面法線計算-----------------------------------------------------------------------------------------

	_NODISCARD static inline auto SurfaceNormal(
		const DirectX::XMFLOAT3& base_pos, const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
	{
		using DirectX::XMLoadFloat3;

		DirectX::XMFLOAT3 normal{};
		const DirectX::XMFLOAT3&& vec1{ pos1.x - base_pos.x, pos1.y - base_pos.y, pos1.z - base_pos.z };
		const DirectX::XMFLOAT3&& vec2{ pos2.x - base_pos.x, pos2.y - base_pos.y, pos2.z - base_pos.z };

		DirectX::XMStoreFloat3(&normal,
			DirectX::XMVector3Normalize(DirectX::XMVector3Cross(XMLoadFloat3(&vec1), XMLoadFloat3(&vec2))));

		return normal;
	}

	// 角度の単位を変更-----------------------------------------------------------------------------------------

	_NODISCARD static inline constexpr auto ToRadian(const DirectX::XMFLOAT3& degree)
	{
		DirectX::XMFLOAT3 rv{};

		rv.x = DirectX::XMConvertToRadians(degree.x);
		rv.y = DirectX::XMConvertToRadians(degree.y);
		rv.z = DirectX::XMConvertToRadians(degree.z);

		return rv;
	}

	_NODISCARD static inline constexpr auto ToRadian(const DirectX::XMFLOAT2& degree)
	{
		DirectX::XMFLOAT2 rv{};

		rv.x = DirectX::XMConvertToRadians(degree.x);
		rv.y = DirectX::XMConvertToRadians(degree.y);

		return rv;
	}

	_NODISCARD static inline constexpr auto ToDegrees(const DirectX::XMFLOAT3& radian)
	{
		DirectX::XMFLOAT3 rv{};

		rv.x = DirectX::XMConvertToDegrees(radian.x);
		rv.y = DirectX::XMConvertToDegrees(radian.y);
		rv.z = DirectX::XMConvertToDegrees(radian.z);

		return rv;
	}

	_NODISCARD static inline constexpr auto ToDegrees(const DirectX::XMFLOAT2& radian)
	{
		DirectX::XMFLOAT2 rv{};

		rv.x = DirectX::XMConvertToDegrees(radian.x);
		rv.y = DirectX::XMConvertToDegrees(radian.y);

		return rv;
	}

	_NODISCARD static inline constexpr auto ToRadian(const float deg_x, const float deg_y, const float deg_z)
	{
		DirectX::XMFLOAT3 rv{};

		rv.x = DirectX::XMConvertToRadians(deg_x);
		rv.y = DirectX::XMConvertToRadians(deg_y);
		rv.z = DirectX::XMConvertToRadians(deg_z);

		return rv;
	}

	_NODISCARD static inline constexpr auto ToRadian(const float deg_x, const float deg_y)
	{
		DirectX::XMFLOAT2 rv{};

		rv.x = DirectX::XMConvertToRadians(deg_x);
		rv.y = DirectX::XMConvertToRadians(deg_y);

		return rv;
	}

	_NODISCARD static inline constexpr auto ToDegrees(const float rad_x, const float rad_y, const float rad_z)
	{
		DirectX::XMFLOAT3 rv{};

		rv.x = DirectX::XMConvertToDegrees(rad_x);
		rv.y = DirectX::XMConvertToDegrees(rad_y);
		rv.z = DirectX::XMConvertToDegrees(rad_z);

		return rv;
	}

	_NODISCARD static inline constexpr auto ToDegrees(const float rad_x, const float rad_y)
	{
		DirectX::XMFLOAT2 rv{};

		rv.x = DirectX::XMConvertToDegrees(rad_x);
		rv.y = DirectX::XMConvertToDegrees(rad_y);

		return rv;
	}

	// 基準座標から他の座標への角度を求める--------------------------------------------------------------

	_NODISCARD static inline float PosToRadian(const DirectX::XMFLOAT2& base_pos,
		const DirectX::XMFLOAT2& another_pos)
	{
		return ::atan2f(another_pos.x - base_pos.x, another_pos.y - base_pos.y);
	}

	// 距離を求める-------------------------------------------------------------------------------

	_NODISCARD static inline float Distance(const DirectX::XMFLOAT2& pos1, const DirectX::XMFLOAT2& pos2)
	{
		return (::sqrtf(::powf(pos2.x - pos1.x, 2.f) + ::powf(pos2.y - pos1.y, 2.f)));
	}

	_NODISCARD static inline float Distance(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
	{
		return (::sqrtf(::powf(pos2.x - pos1.x, 2.f) + ::powf(pos2.y - pos1.y, 2.f) + ::powf(pos2.z - pos1.z, 2.f)));
	}

	// 距離の二乗を求める-------------------------------------------------------------------------------

	_NODISCARD static inline float DistanceSq(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
	{
		return (::powf(pos2.x - pos1.x, 2.f) + ::powf(pos2.y - pos1.y, 2.f) + ::powf(pos2.z - pos1.z, 2.f));
	}

	_NODISCARD static inline float DistanceSq(const DirectX::XMFLOAT2& pos1, const DirectX::XMFLOAT2& pos2)
	{
		return (::powf(pos2.x - pos1.x, 2.f) + ::powf(pos2.y - pos1.y, 2.f));
	}

	// 二点間の中点を求める----------------------------------------------------------------------------------

	_NODISCARD static inline DirectX::XMFLOAT2 MiddlePoint(const DirectX::XMFLOAT2& pos1, const DirectX::XMFLOAT2& pos2)
	{
		return { ((pos1.x + pos2.x) / 2.f), ((pos1.y + pos2.y) / 2.f) };
	}

	_NODISCARD static inline DirectX::XMFLOAT3 MiddlePoint(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
	{
		return  { ((pos1.x + pos2.x) / 2.f), ((pos1.y + pos2.y) / 2.f), ((pos1.x + pos2.x) / 2.f) };
	}

	// 二点間の距離を求める（簡易版）--------------------------------------------------------------------------

	_NODISCARD static inline float EasyDistance(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
	{
		auto FAbs{ [](const auto& vf3, const auto& vf3b) {
			DirectX::XMFLOAT3 temp{};

			temp.x = fabs(vf3.x - vf3b.x);
			temp.y = fabs(vf3.y - vf3b.y);
			temp.z = fabs(vf3.z - vf3b.z);

			return temp;
		} };

		const auto&& dis_vec3{ FAbs(pos2, pos1) };

		return (dis_vec3.x + dis_vec3.y + dis_vec3.z);
	}

	_NODISCARD static inline float EasyDistance(const DirectX::XMFLOAT2& pos1, const DirectX::XMFLOAT2& pos2)
	{
		auto FAbs{ [](const auto& vf3, const auto& vf3b) {
			DirectX::XMFLOAT2 temp{};

			temp.x = fabs(vf3.x - vf3b.x);
			temp.y = fabs(vf3.y - vf3b.y);

			return temp;
		} };

		const auto&& dis_vec3{ FAbs(pos2, pos1) };

		return (dis_vec3.x + dis_vec3.y);
	}

	// 値が誤差を含んだ上で同じかどうか-----------------------------------------------------------------------

	_NODISCARD static inline bool NearyEqual(
		const DirectX::XMFLOAT2& vec1, const DirectX::XMFLOAT2& vec2, const float error_num)
	{
		auto FAbs{ [](const auto& vf3, const auto& vf3b)
		{
			DirectX::XMFLOAT2 temp{};

			temp.x = fabs(vf3.x - vf3b.x);
			temp.y = fabs(vf3.y - vf3b.y);

			return temp;
		} };

		auto dis{ FAbs(vec1, vec2) };

		return (dis.x <= error_num && dis.y <= error_num);
	}

	_NODISCARD static inline bool NearyEqual(
		const DirectX::XMFLOAT3& vec1, const DirectX::XMFLOAT3& vec2, const float error_num)
	{
		auto FAbs{ [](const auto& vf3, const auto& vf3b)
		{
			DirectX::XMFLOAT3 temp{};

			temp.x = fabs(vf3.x - vf3b.x);
			temp.y = fabs(vf3.y - vf3b.y);
			temp.z = fabs(vf3.z - vf3b.z);

			return temp;
		} };

		auto dis{ FAbs(vec1, vec2) };

		return (dis.x <= error_num && dis.y <= error_num && dis.z <= error_num);
	}

	_NODISCARD static inline bool NearyEqual(
		const DirectX::XMFLOAT4& vec1, const DirectX::XMFLOAT4& vec2, const float error_num)
	{
		auto FAbs{ [](const auto& vf3, const auto& vf3b)
		{
			DirectX::XMFLOAT4 temp{};

			temp.x = fabs(vf3.x - vf3b.x);
			temp.y = fabs(vf3.y - vf3b.y);
			temp.z = fabs(vf3.z - vf3b.z);
			temp.z = fabs(vf3.w - vf3b.w);

			return temp;
		} };

		auto dis{ FAbs(vec1, vec2) };

		return (dis.x <= error_num && dis.y <= error_num && dis.z <= error_num && dis.w <= error_num);
	}
}