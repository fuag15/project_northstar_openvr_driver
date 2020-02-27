#pragma once

#include <memory>

#include <LeapC.h>

#include "math/ISkeletalAdapter.hpp"

#include "driver/Settings.hpp"
#include "driver/Types.hpp"
#include "math/IVectorFactory.hpp"
#include "math/IMatrixFactory.hpp"
#include "math/Types.hpp"

namespace northstar {
    namespace math {
        class CSkeletalAdapter : public ISkeletalAdapter {
        public:
            CSkeletalAdapter(std::shared_ptr<IVectorFactory> pVectorFactory, std::shared_ptr<IMatrixFactory> pMatrixFactory);
            ~CSkeletalAdapter() = default;
            virtual void FromLeapMotionHandToOVRBonePoseArray(
                const vr::DriverPose_t& sPose,
                const types::AffineMatrix4d& m4dFromLeapSensorToHMDRelativeSpace,
                const types::AffineMatrix4d& m4dFromHMDToWorldSpace,
                const LEAP_HAND& sLeapHand, 
                std::array<vr::VRBoneTransform_t, northstar::driver::settings::values::driverConfiguration::k_unBoneCount>& saOpenVRBoneTargets) const override final;
        private:
            static constexpr bool x_dIgnorePalmOffset = false; // TODO: read from config
            void WriteConvertedDigitToTargetOVRBoneIndicies(
                const types::AffineMatrix4d& m4dFromWorldToRootBoneRelativeSpace,
                const types::AffineMatrix4d& m4dFromLeapSensorToHMDRelativeSpace,
                const types::AffineMatrix4d& m4dFromHMDToWorldSpace,
                const LEAP_DIGIT& sLeapDigit,
                const std::array<northstar::driver::types::EHandeSkeletonBone, 5>& aeDestinationIndicies,
                const northstar::driver::types::EHandeSkeletonBone& eAuxBoneIndex,
                std::array<vr::VRBoneTransform_t, northstar::driver::settings::values::driverConfiguration::k_unBoneCount>& saOpenVRBoneTargets) const;

            types::AffineMatrix4d northstar::math::CSkeletalAdapter::ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
                const types::AffineMatrix4d& m4dFromWorldSpaceToParentRelativeSpace,
                const types::AffineMatrix4d& m4dFromLeapSensorToHMDRelativeSpace,
                const types::AffineMatrix4d& m4dFromHMDToWorldSpace,
                const LEAP_VECTOR& sLeapPosition,
                const LEAP_QUATERNION& sLeapOrientation,
                vr::VRBoneTransform_t& sVRBoneTransform) const;

            types::Vector4d GetTranslationSensitiveV4DFromLeapVectorPosition(const LEAP_VECTOR& sPosition) const;
            types::Quaterniond GetQuaterniondFromLeapQuaternionOrientation(const LEAP_QUATERNION& sOrientation) const;
            void WritePositionAndOrientationIntoVRBonePose(vr::VRBoneTransform_t& sVRBoneTransform, const types::Vector4d& v4dPosition, const types::Quaterniond& qdOrientation) const;

            std::shared_ptr<IVectorFactory> m_pVectorFactory;
            std::shared_ptr<IMatrixFactory> m_pMatrixFactory;
        };
    }
}