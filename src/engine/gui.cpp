#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gui.hpp"

void centeredCheckbox(const char* label, bool* value, float window_width) {
    float width = ImGui::GetFrameHeight() + ImGui::GetStyle().ItemInnerSpacing.x + ImGui::CalcTextSize(label).x;
    ImGui::SetCursorPosX((window_width - width) * 0.5f);
    ImGui::Checkbox(label, value);
}

void Gui::pauseMenu(ImGuiIO& io, State& state, Client& client) {
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowBgAlpha(0.5f);
    
    ImGui::Begin("PauseOverlay", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar);

    ImGui::Dummy(ImVec2(io.DisplaySize.x * 0.25f, 0.0f));

    float window_width = ImGui::GetWindowSize().x;
    
    ImGui::PushFont(font_title);

    float text_width = ImGui::CalcTextSize("Paused").x;
    ImGui::SetCursorPosX((window_width - text_width) * 0.5f);
    ImGui::Text("Paused");

    ImGui::PopFont();

    ImGui::PushFont(font_default);

    ImGui::Dummy(ImVec2(0.0f, 32.0f));

    centeredCheckbox("VSync", &state.settings.vsync, window_width);
    centeredCheckbox("Bloom (Reccomended)", &state.settings.bloom, window_width);
    centeredCheckbox("Show FPS", &state.settings.FPS, window_width);

    ImGui::Dummy(ImVec2(0.0f, 32.0f));

    float button_width = ImGui::CalcTextSize("Menu").x * 4.0f;
    ImGui::SetCursorPosX((window_width - button_width) * 0.5f);
    if (ImGui::Button("Menu", ImVec2(button_width, 0))) {
        if (state.gameMode == MULTIPLAYER) client.disconnect();
        state.scene = MENU;
    }

    ImGui::PopFont();

    ImGui::End();
}

void Gui::startMenu(ImGuiIO& io, State& state) {
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.1f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("MenuOverlay", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar);

    ImGui::Image((ImTextureID) (intptr_t) titleTexture.getID(), ImVec2(titleTexture.width * 0.25, titleTexture.height * 0.25), {0, 1}, {1, 0});
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.75f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("StartOverlay", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar);

    float window_width = ImGui::GetWindowSize().x;

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(204, 51, 204, 100));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(204, 51, 204, 100));

    ImGui::PushFont(font_title);
    ImGui::SetCursorPosX((window_width - ImGui::CalcTextSize("Start").x * 4.0f) * 0.5f);
    
    if (ImGui::Button("Start", ImVec2(ImGui::CalcTextSize("Start").x * 4.0f, 0))) {
        state.mouseState = NORMAL;
        state.scene = SELECT;
    }

    ImGui::PopFont();

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    ImGui::End();
}

static bool multiplayerOverlay = false;
static std::string address = "localhost";
static int port = 3000;
static std::string username = "Player";

void Gui::selectMenu(ImGuiIO& io, State& state, Client& client) {
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("SelectOverlay", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar);

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(204, 51, 204, 100));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(220, 25, 255, 100));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(220, 25, 255, 100));
    ImGui::PushFont(font_title);
    
    float window_width = ImGui::GetWindowSize().x;
    float button_width = ImGui::CalcTextSize("XXXXXXXXXXXX").x * 3.0f;


    ImGui::SetCursorPosX((window_width - button_width) * 0.5f);
    if (ImGui::Button("Singeplayer", ImVec2(button_width, 0))) {
        state.mouseState = LOCKED;
        state.gameMode = SINGLEPLAYER;

        state.scene = GAME;
    }

    ImGui::SetCursorPosX((window_width - button_width) * 0.5f);
    if (ImGui::Button("Multiplayer", ImVec2(button_width, 0)) && !multiplayerOverlay) {
        multiplayerOverlay = true;
    }

    if (multiplayerOverlay) {
        state.mouseState = NORMAL;
        state.gameMode = MULTIPLAYER;

        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
        ImGui::SetNextWindowBgAlpha(1.0f);
        
        ImGui::Begin("MultiplayerConnectOverlay", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar);

        ImGui::InputText("Address", &address);
        ImGui::InputInt("Port", &port);
        ImGui::InputText("Username", &username);

        ImGui::Dummy(ImVec2(0.0f, io.DisplaySize.x * 0.1f));

        float ww = ImGui::GetWindowSize().x;
        float bw = ImGui::CalcTextSize("XXXX").x * 4.0f;

        ImGui::SetCursorPosX((ww - bw) * 0.5f);
        if (ImGui::Button("Join", ImVec2(bw, 0))) {
            client.connect(address, port, username);
            state.mouseState = LOCKED;
            state.scene = GAME;
        }

        ImGui::SetCursorPosX((ww - bw) * 0.5f);
        if (ImGui::Button("Back", ImVec2(bw, 0))) {
            multiplayerOverlay = false;
        }

        ImGui::End();
    }

    ImGui::SetCursorPosX((window_width - button_width) * 0.5f);
    if (ImGui::Button("Settings", ImVec2(button_width, 0))) {

    }

    ImGui::SetCursorPosX((window_width - button_width) * 0.5f);
    if (ImGui::Button("Quit", ImVec2(button_width, 0))) {
        state.scene = QUIT;
    }

    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    ImGui::End();
}

static std::string chatMessage = "";
static bool chatFocused = false;

void Gui::chatbox(ImGuiIO& io, [[maybe_unused]] State& state, Client& client) {

    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 10.f, io.DisplaySize.y - 10.f), ImGuiCond_Always, ImVec2(1.0f, 1.0f));
    ImGui::SetNextWindowBgAlpha(0.25f);

    ImGui::PushFont(font_small);

    ImGui::Begin("ChatOverlay", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove
    );

    ImVec2 childSize = ImVec2(0.0f, 150.f);
    ImGui::BeginChild("ChatArea", childSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    
    float dummyHeight = 150.f - (client.getChatMessages().size() * 15.f);
    dummyHeight = std::clamp(dummyHeight, 50.f, 150.f);
    ImGui::Dummy(ImVec2(0.f, dummyHeight));
    
    for (const std::string& message : client.getChatMessages()) {
        ImGui::Text("%s", message.c_str());
    }

    float scrollY = ImGui::GetScrollY();
    float maxScrollY = ImGui::GetScrollMaxY();

    if (scrollY == maxScrollY) {
        ImGui::SetScrollHereY(1.0f);
    }
    
    ImGui::EndChild();

    if (ImGui::IsKeyPressed(ImGuiKey_T) && !chatFocused) {
        ImGui::SetKeyboardFocusHere();
        chatFocused = true;
    }
    if (ImGui::IsKeyReleased(ImGuiKey_T)) {
        chatFocused = false;
    }

    ImGui::Text("Chat:");
    ImGui::SameLine();
    ImGui::InputText("##chat", &chatMessage);

    if (ImGui::IsItemDeactivated() && ImGui::IsKeyPressed(ImGuiKey_Enter) && !chatMessage.empty()) {
        client.emitChatMessage(chatMessage);
        chatMessage.clear();
    }
    
    ImGui::PopFont();

    ImGui::End();
}

Gui::Gui(Window& window) : titleTexture {"res/textures/title.png", LINEAR} {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGuiStyle& style = ImGui::GetStyle();
    
    float rounding = 5.f;
    style.TabRounding = rounding;
    style.FrameRounding = rounding;
    style.GrabRounding = rounding;
    style.WindowRounding = rounding;
    style.PopupRounding = rounding;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    font_title_large = io.Fonts->AddFontFromFileTTF("res/fonts/default_font.ttf", 60.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    font_title = io.Fonts->AddFontFromFileTTF("res/fonts/default_font.ttf", 40.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    font_small = io.Fonts->AddFontFromFileTTF("res/fonts/default_font.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    font_default = io.Fonts->AddFontFromFileTTF("res/fonts/default_font.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

Gui::~Gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

float counter = 0;
float FPS = 0;

void Gui::update(Client& client, State& state, float dt) {
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::PushFont(font_default);
    
    if (state.settings.FPS) {
        ImGui::Begin("FPSOverlay", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoInputs);

        counter += dt;
        if (counter >= 0.5) {
            counter = 0;
            FPS = 1.0 / dt;
        }
        
        ImGui::Text("FPS: %.f", FPS);
        ImGui::End();
    }

    ImGui::PopFont();

    if (state.scene == PAUSED) pauseMenu(io, state, client);
    else if (state.scene == MENU) {
        startMenu(io, state);
    } else if (state.scene == SELECT) {
        selectMenu(io, state, client);
    } else if (state.scene == GAME && state.gameMode == MULTIPLAYER) {
        chatbox(io, state, client);
    }
}

void Gui::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}