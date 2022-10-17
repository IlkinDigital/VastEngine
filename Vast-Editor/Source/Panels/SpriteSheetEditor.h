#pragma once

#include "GUI/Panels/Panel.h"
#include "AssetManager/AssetTypes/BoardSpriteSheetAsset.h"

namespace Vast {

	class SpriteSheetView : public Panel
	{
	public:
		SpriteSheetView()
			: Panel("View") {}

		void SetSpriteSheet(const Ref<Board2D::SpriteSheet>& spriteSheet) { m_SpriteSheet = spriteSheet; }

		virtual void DrawPanel() override;
	private:
		Ref<Board2D::SpriteSheet> m_SpriteSheet;
	};

	class SpriteSheetSettings : public Panel
	{
	public:
		SpriteSheetSettings()
			: Panel("Settings") {}

		void SetSpriteSheet(const Ref<BoardSpriteSheetAsset>& spriteSheet) { m_SpriteSheet = spriteSheet; }

		virtual void DrawPanel() override;
	private:
		Ref<Board2D::Sprite> m_Extracted;
		Ref<BoardSpriteSheetAsset> m_SpriteSheet;
	};

	class SpriteSheetEditor : public Subwindow
	{
	public:
		SpriteSheetEditor();

		virtual void OnAttach() override;

		void SetSpriteSheet(const Ref<BoardSpriteSheetAsset>& spriteSheet) 
		{ 
			m_SpriteSheet = spriteSheet;
			m_View.SetSpriteSheet(spriteSheet->GetSpriteSheet());
			m_Settings.SetSpriteSheet(spriteSheet);
		}
		
		virtual void DrawPanel() override;
	private:
		SpriteSheetView m_View;
		SpriteSheetSettings m_Settings;

		bool m_InitializedDockspace = false;

		Ref<BoardSpriteSheetAsset> m_SpriteSheet;
	};

}