#pragma once
#include "UI/UIUtil.hpp"
#include "Singleton.hpp"

#ifdef EDITOR

class TextureData;
class EditorRenderer;
class UIViewport;
class UIFixedContainer;

class Editor : public Singleton<Editor>
{
public:

	void Initialize();

	void Update();
	void Draw();

	GLuint GetSceneTarget();

	void OnWindowResize(ivec2 EditorDimensions);
	void CalculateViewportSize(ivec2 FullWindowDimensions, bool resizeBuffer = false);

	UIViewport* GetViewport() { return m_Viewport; }

private:
	UIViewport* m_Viewport = nullptr;
	UIFixedContainer* m_Root = nullptr;

private:
	//Disable constructors
	friend class Singleton<Editor>;

	Editor();
	virtual ~Editor();

	Editor( const Editor& t );
	Editor& operator=( const Editor& t );
};

#endif