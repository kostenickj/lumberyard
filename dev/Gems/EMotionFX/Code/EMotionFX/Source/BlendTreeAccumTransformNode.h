/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

// include the required headers
#include "EMotionFXConfig.h"
#include "AnimGraphNode.h"


namespace EMotionFX
{
    /**
     *
     *
     *
     */
    class EMFX_API BlendTreeAccumTransformNode
        : public AnimGraphNode
    {
    public:
        AZ_RTTI(BlendTreeAccumTransformNode, "{2216B366-F06C-4742-B998-44F4357F45BE}", AnimGraphNode)
        AZ_CLASS_ALLOCATOR_DECL

        // input ports
        enum
        {
            INPUTPORT_POSE              = 0,
            INPUTPORT_TRANSLATE_AMOUNT  = 1,
            INPUTPORT_ROTATE_AMOUNT     = 2,
            INPUTPORT_SCALE_AMOUNT      = 3
        };

        enum
        {
            PORTID_INPUT_POSE               = 0,
            PORTID_INPUT_TRANSLATE_AMOUNT   = 1,
            PORTID_INPUT_ROTATE_AMOUNT      = 2,
            PORTID_INPUT_SCALE_AMOUNT       = 3
        };

        enum Axis : AZ::u8
        {
            AXIS_X = 0,
            AXIS_Y = 1,
            AXIS_Z = 2
        };

        enum ScaleAxis : AZ::u8
        {
            SCALEAXIS_X = 0,
            SCALEAXIS_Y = 1,
            SCALEAXIS_Z = 2,
            SCALEAXIS_ALL = 3
        };

        // output ports
        enum
        {
            OUTPUTPORT_RESULT   = 0
        };

        enum
        {
            PORTID_OUTPUT_POSE = 0
        };

        class EMFX_API UniqueData
            : public AnimGraphNodeData
        {
            EMFX_ANIMGRAPHOBJECTDATA_IMPLEMENT_LOADSAVE
        public:
            AZ_CLASS_ALLOCATOR_DECL

            UniqueData(AnimGraphNode* node, AnimGraphInstance* animGraphInstance)
                : AnimGraphNodeData(node, animGraphInstance)
            {
                mDeltaTime  = 0.0f;
                mNodeIndex  = MCORE_INVALIDINDEX32;
                mMustUpdate = true;
                mIsValid    = false;
                mAdditiveTransform.Identity();
                EMFX_SCALECODE(mAdditiveTransform.mScale.CreateZero();)
            }

            ~UniqueData() {}

        public:
            Transform   mAdditiveTransform;
            uint32      mNodeIndex;
            float       mDeltaTime;
            bool        mMustUpdate;
            bool        mIsValid;
        };

        BlendTreeAccumTransformNode();
        ~BlendTreeAccumTransformNode();

        void Reinit() override;
        bool InitAfterLoading(AnimGraph* animGraph) override;

        void OnUpdateUniqueData(AnimGraphInstance* animGraphInstance) override;

        uint32 GetVisualColor() const override                  { return MCore::RGBA(255, 0, 0); }
        bool GetCanActAsState() const override                  { return false; }
        bool GetSupportsVisualization() const override          { return true; }
        bool GetHasOutputPose() const override                  { return true; }

        AnimGraphPose* GetMainOutputPose(AnimGraphInstance* animGraphInstance) const override     { return GetOutputPose(animGraphInstance, OUTPUTPORT_RESULT)->GetValue(); }

        const char* GetPaletteName() const override;
        AnimGraphObject::ECategory GetPaletteCategory() const override;

        void SetTargetNodeName(const AZStd::string& targetNodeName);
        void SetTranslationAxis(Axis axis);
        void SetRotationAxis(Axis axis);
        void SetScaleAxis(ScaleAxis axis);
        void SetTranslateSpeed(float speed);
        void SetRotateSpeed(float speed);
        void SetScaleSpeed(float speed);
        void SetInvertTranslation(bool invertTranslation);
        void SetInvertRotation(bool invertRotation);
        void SetInvertScale(bool invertScale);

        static void Reflect(AZ::ReflectContext* context);

    private:
        void UpdateUniqueData(AnimGraphInstance* animGraphInstance, UniqueData* uniqueData);
        void Output(AnimGraphInstance* animGraphInstance) override;
        void Update(AnimGraphInstance* animGraphInstance, float timePassedInSeconds) override;

        void OnAxisChanged();

        AZStd::string   m_targetNodeName;
        float           m_translateSpeed;
        float           m_rotateSpeed;
        float           m_scaleSpeed;
        Axis            m_translationAxis;
        Axis            m_rotationAxis;
        ScaleAxis       m_scaleAxis;
        bool            m_invertTranslation;
        bool            m_invertRotation;
        bool            m_invertScale;
    };
} // namespace EMotionFX
