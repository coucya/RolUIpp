#pragma once

namespace RolUI {

    class Window;

    class Application {
      public:
        Application();

        Application(const Application&) = delete;
        Application(Application&&) = delete;

        ~Application();

        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        void set_window(Window* w) noexcept;
        void remove_window(Window* w) noexcept;

        void run() noexcept;
        void exit() noexcept;

      private:
        void _draw_window() noexcept;
        void _dispatch_event() noexcept;

      private:
        bool _should_exit = false;
        Window* _window = nullptr;
    };

} // namespace RolUI
