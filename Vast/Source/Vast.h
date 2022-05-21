#pragma once

// ---- Application ------------------------
#include "ApplicationCore/Application.h"
#include "Input/Input.h"
// -----------------------------------------

// ---- Core -------------------------------
#include "Core/CoreMinimal.h"
// -----------------------------------------

// ---- Math -------------------------------
#include "Core/Math/Math.h"
#include "Core/Math/MathOperations.h"
#include "Core/Math/MathUtils.h"
// -----------------------------------------

// ---- Events -----------------------------
#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/EventDispatcher.h"
// -----------------------------------------

// ---- Graphics ---------------------------
#include "Renderer/RendererCommand.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Framebuffer.h"
// -----------------------------------------

// ---- Scene ------------------------------
#include "Scene/SceneCore.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
// -----------------------------------------

// ---- Other ------------------------------
#include "Camera/OrthographicCamera.h"
#include "Camera/PerspectiveCamera.h"
#include "Camera/EditorCamera.h"
// -----------------------------------------