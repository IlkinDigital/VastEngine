#include "DebugWindow.h"

#include "EditorLayout/Layout.h"
#include "DebugLayer.h"

#include <imgui_internal.h>

namespace Vast {

    class DebugImage : public Subwindow
    {
    public:
        DebugImage()
            : Subwindow("Debug Image")
        {
            m_ImageName = "Debug View###" + std::to_string(GetUUID());
        }

        DebugImage(const Ref<Texture2D>& texture, const Vector2& uv0, const Vector2& uv1)
            : DebugImage() 
        {
            m_Texture = texture;
            m_UV0 = uv0;
            m_UV1 = uv1;
        }

        void SetTexture(const Ref<Texture2D>& texture) { m_Texture = texture; }
        void SetUV0(const Vector2& uv0) { m_UV0 = uv0; }
        void SetUV1(const Vector2& uv1) { m_UV1 = uv1; }

        virtual void DrawPanel() override
        {
            if (!EditorLayout::BeginWindow(*this, &m_IsOpen))
            {
                EditorLayout::EndWindow();
                return;
            }

            if (!m_InitializedDockspace)
            {
                ImGui::SetWindowSize({ (float)400, (float)400 });

                ImGuiID dockspaceID = GetUUID();

                ImGui::DockBuilderRemoveNode(dockspaceID);
                ImGui::DockBuilderAddNode(dockspaceID,
                    ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_PassthruCentralNode);
                ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);

                ImGui::DockBuilderDockWindow(m_ImageName.c_str(), dockspaceID);

                ImGui::DockBuilderFinish(dockspaceID);
                m_InitializedDockspace = true;
            }

            ImGui::Begin(m_ImageName.c_str());

            if (m_Texture)
            {
                ImGui::Image((ImTextureID)m_Texture->GetRendererID(),
                    { (float)m_Texture->GetWidth(), (float)m_Texture->GetHeight() },
                    { m_UV0.y, m_UV0.x }, { m_UV1.y, m_UV1.x });
            }

            ImGui::End();

            EditorLayout::EndWindow();
        }
    private:
        Ref<Texture2D> m_Texture;
        Vector2 m_UV0 = { 0, 1 };
        Vector2 m_UV1 = { 1, 0 };
        bool m_InitializedDockspace = false;
        String m_ImageName;
    };

    void DebugOutput::Image(const Ref<Texture2D>& texture, const Vector2& uv0, const Vector2& uv1)
    {
        auto debugImage = CreateRef<DebugImage>(texture, uv0, uv1);
        DebugLayer::AddDebugSW(debugImage);
    }

}