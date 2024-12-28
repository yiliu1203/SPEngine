#include "Application.h"
#include "Engine.h"
#include "Event/ApplicationEvent.h"
#include "Event/Event.h"
#include "Graphics/Renderer.h"
#include "Misc/Input.h"



namespace SP {

using namespace RHI;
Application* app = nullptr;

bool OnEventTest(Event& e);


Application::Application()
    : _IsRunning(false)
{
    app = this;
}

bool Application::InitMainWindow()
{
    Global::g_input  = Input::s_instance;
    _Window          = std::unique_ptr<Window>(Window::Create());
    Global::g_window = _Window.get();
    // Global::g_input->AddEventListener(Input::EventDelegateType::FromMethod<Application, &Application::OnEvent>(this));
    ADD_LISTENER(Input, Application, OnEvent);
    return true;
}

bool Application::InitRenderer()
{
    Renderer::Init();
    // _renderer = Renderer::Create();
    _engine = new Engine();
    _engine->Initialize();
    _ImGuiLayer = new ImGuiLayer("imgui");
    PushOverLayer(_ImGuiLayer);
    return true;
}



Application::~Application()
{
    for (auto iter = _LayerStack.begin(); iter != _LayerStack.end(); iter++) {
        (*iter)->OnDetach();
    }
}

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

    for (auto it = _LayerStack.end(); it != _LayerStack.begin();) {
        (*--it)->OnEvent(event);
        if (event.Handled) break;
    }
    _engine->OnEvent(event);
    return true;
}

void Application::PushLayer(Layer* layer)
{
    return;
    _LayerStack.PushLayer(layer);
    layer->_app = this;
    layer->OnAttach();
}

void Application::PushOverLayer(Layer* layer)
{
    return;
    _LayerStack.PushOverlay(layer);
    layer->_app = this;
    layer->OnAttach();
}

const Layer* Application::GetLayer(const std::string& layername)
{
    for (auto* layer : _LayerStack) {
        if (layer->GetName() == layername) {
            return layer;
        }
    }
    return nullptr;
}

Application* Application::Get()
{
    return app;
}

bool Application::OnWindowResize(const WindowResizeEvent& e)
{
    if (e.GetWidth() == 0 || e.GetHeight() == 0) {
        _Minimized = true;
        return false;
    }

    _Minimized = false;
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
    return true;
}


void Application::Run()
{

    if (!_IsRunning) {
        _IsRunning = true;
        // todo for dev dx11
        // for (Layer* layer : _LayerStack) layer->OnPrepared();
    }

    while (_IsRunning) {
        if (_Window->ProcessMessage() == -1) {
            _IsRunning = false;
            break;
        }
        // todo for dev dx11
        _engine->RunFrame();
        // for (Layer* layer : _LayerStack) layer->OnUpdate();

        // _ImGuiLayer->Begin();
        // for (Layer* layer : _LayerStack) layer->OnImGuiRenderer();
        // _ImGuiLayer->End();

        // SP_INFO("mouse_pos: {0},{1}", Input::GetMousePositionX(), Input::GetMousePositionY());
        _Window->SwapChains();
    }
}

void Application::Close()
{
    _IsRunning = false;
}



}   // namespace SP