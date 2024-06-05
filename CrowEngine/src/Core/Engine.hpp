#ifndef ENGINE_HPP
#define ENGINE_HPP

namespace crowe
{
class Engine {
public:
  Engine();

  ~Engine();

private:
  static void InitializeEngine();

  static void HandleInput();

  static void Update();

  static void Render();

  static void Cleanup();
};
}

#endif //ENGINE_HPP
