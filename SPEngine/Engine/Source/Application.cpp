#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Event/Event.h"
#include "Misc/Input.h"
#include "Window/IWindow.h"



namespace SP {

Application* app = nullptr;
bool         OnEventTest(Event& e);


Application::Application()
    : m_running(false)
{
    app = this;
}

bool Application::InitMainWindow()
{
    m_window = std::unique_ptr<IWindow>(IWindow::Create());
    // Global::g_input->AddEventListener(Input::EventDelegateType::FromMethod<Application, &Application::OnEvent>(this));
    ADD_LISTENER(Input, Application, OnEvent);
    return true;
}

bool Application::InitRenderer()
{
    return true;
}



Application::~Application() {}

bool Application::OnEvent(Event& event)
{
    SP_INFO("Event({0})", event);

    EventDispatcher eventDisPatcher(event);
    eventDisPatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) -> bool {
        Close();
        return true;
    });

    eventDisPatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) -> bool {
        OnWindowResize(e);
        return false;
    });
    return true;
}


Application* Application::Get()
{
    return app;
}

bool Application::OnWindowResize(const WindowResizeEvent& e)
{
    if (e.GetWidth() == 0 || e.GetHeight() == 0) {
        m_minimized = true;
        return false;
    }

    m_minimized = false;
    return true;
}


void Application::Run()
{

    if (!m_running) {
        m_running = true;
        // todo for dev dx11
        // for (Layer* layer : _LayerStack) layer->OnPrepared();
    }

    while (m_running) {
        if (m_window->ProcessMessage() == -1) {
            m_running = false;
            break;
        }
        // todo for dev dx11
        // _engine->RunFrame();
        // for (Layer* layer : _LayerStack) layer->OnUpdate();

        // _ImGuiLayer->Begin();
        // for (Layer* layer : _LayerStack) layer->OnImGuiRenderer();
        // _ImGuiLayer->End();

        // SP_INFO("mouse_pos: {0},{1}", Input::GetMousePositionX(), Input::GetMousePositionY());
        // _Window->SwapChains();
    }
}

void Application::Close()
{
    m_running = false;
}



}   // namespace SP