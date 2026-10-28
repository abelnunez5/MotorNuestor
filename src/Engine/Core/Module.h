#pragma once

class Application;

enum class UpdateStatus { Continue, Stop, Error };

// Base class for every engine module. Application owns them, calls the phases in registration order and CleanUp() in reverse order.
class Module {
public:
    explicit Module(Application& app) : app(app) {}
    virtual ~Module() = default;

    Module(const Module&) = delete;
    Module& operator=(const Module&) = delete;

    virtual bool Init() { return true; }
    virtual UpdateStatus PreUpdate() { return UpdateStatus::Continue; }
    virtual UpdateStatus Update() { return UpdateStatus::Continue; }
    virtual UpdateStatus PostUpdate() { return UpdateStatus::Continue; }

    // May be called even if Init() failed or never ran: it must be safe.
    virtual void CleanUp() {}

protected:
    Application& app;
};
