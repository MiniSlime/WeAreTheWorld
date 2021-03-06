# include <Siv3D.hpp>
# include "../sayHi/sayHi.hpp"

struct RingEffect : IEffect {
    Vec2 m_pos;

    RingEffect(const Vec2& pos)
        : m_pos(pos) {}

    bool update(double t) override {
        const double e = EaseOutExpo(t);
        //Print << e;

        Circle(m_pos, e * 100).drawFrame(20.0 * (1.0 - e), RandomColor());

        return t < 1.0;
    }
};

void Main()
{
    Window::SetTitle(U"We Are The World");
    Window::SetStyle(WindowStyle::Sizable);
    Window::Resize(Size(1280, 640)),
    Scene::SetBackground(Palette::White);
    Scene::SetLetterbox(Palette::Black);

    const Texture earth(Emoji(U"🌏"));
    const Texture texture(Emoji(U"🐈"));
    const Texture muscle(Emoji(U"💪"));
    const Texture dancer(Emoji(U"🤔"));
      
    const Vec2 center = Scene::Center();
    constexpr double r = 200.0;

    Array<Vec2> points;
    const Font font(50,Typeface::Default,FontStyle::BoldItalic);

    double sliderValue = 0.0;

    TextEditState text1;
    text1.text = U"Enter text";

    Camera2D camera(Vec2(0,0), 1.0, Camera2DParameters::MouseOnly());
    camera.setCenter(center);
    camera.setTargetCenter(center);

    Effect effect;

    while (System::Update())
    {
        {
            camera.update();
            const auto trans = camera.createTransformer();

            const double t = Scene::Time();
            const double dt = Scene::DeltaTime();

            earth.rotated(t).drawAt(center);

            if (MouseL.down()) {
                points << Cursor::Pos();
                sayHi();
            }

            static double baseTheta = 0;
            baseTheta += dt * 30_deg * sliderValue;
            for (auto i : step(12)) {
                const double theta = i * 30_deg + baseTheta;
                const Vec2 pos = OffsetCircular(center, r, theta);
                const Vec2 pos2 = OffsetCircular(center, 110, theta * -1);
                const Vec2 pos3 = OffsetCircular(center, 300, theta * -1);

                texture.flipped(i % 2).rotated(t + i).drawAt(pos);
                muscle.flipped(i % 2).scaled(0.6).rotated(t + i).drawAt(pos2);
                dancer.flipped(i % 2).scaled(1.5).rotated(t + i).drawAt(pos3);
            }

            for (const auto& point : points) {
                Circle(point, 10).draw(Palette::Aqua);
            }

            SimpleGUI::TextBox(text1, Vec2(center.x - 200, center.y + 170), 400);

            const String text = text1.text;
            const size_t length = static_cast<size_t>((int)(t / 0.1) % (text.length() + 1));

            font(text.substr(0, length)).drawAt(center.movedBy(4, 4), Palette::Black);
            font(text.substr(0, length)).drawAt(center, Palette::Red);

            if (SimpleGUI::Button(U"push here", Vec2(center.x - 50, center.y + 70), 100)) {
                Scene::SetBackground(RandomColor());
                Scene::SetLetterbox(RandomColor());
            }
            SimpleGUI::Slider(U"{:.2f}"_fmt(sliderValue), sliderValue, -10.0, 10.0, Vec2(center.x - 100, center.y + 120));
            
            if (MouseR.down()) {
                effect.add<RingEffect>(Cursor::Pos());
            }
            effect.update();
        }

        if (SimpleGUI::Button(U"Reset", Vec2(20, 20)))
        {
            // 2D カメラのパラメータをリセット
            camera.setCenter(center);
            camera.setTargetCenter(center);
            camera.setScale(1.0);
            camera.setTargetScale(1.0);
        }
        camera.draw(Palette::Red);
    }
}