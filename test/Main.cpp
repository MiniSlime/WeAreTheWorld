# include <Siv3D.hpp>

void Main()
{
    Scene::SetBackground(Palette::White);

    const Texture earth(Emoji(U"🌏"));
    const Texture texture(Emoji(U"🐈"));
    const Texture muscle(Emoji(U"💪"));
    const Texture dancer(Emoji(U"🤔"));
      
    const Vec2 center = Scene::Center();
    constexpr double r = 200.0;

    Array<Vec2> points;
    const Font font(50,Typeface::Default,FontStyle::BoldItalic);

    while (System::Update())
    {
        const double t = Scene::Time();

        earth.rotated(t).drawAt(center);

        if (MouseL.down()) {
            points << Cursor::Pos();
        }

        for (auto i : step(12)) {
            const double theta = i * 30_deg + t * 30_deg;
            const Vec2 pos = OffsetCircular(center,r,theta);
            const Vec2 pos2 = OffsetCircular(center, 110, theta*-1);
            const Vec2 pos3 = OffsetCircular(center, 300, theta*-1);

            texture.flipped(i % 2).rotated(t + i).drawAt(pos);
            muscle.flipped(i % 2).scaled(0.6).rotated(t + i).drawAt(pos2);
            dancer.flipped(i % 2).scaled(1.5).rotated(t + i).drawAt(pos3);
        }

        for (const auto& point : points) {
            Circle(point,10).draw(Palette::Aqua);
        }

        const String text = U"いそちゃんを救う会";
        const size_t length = static_cast<size_t>((int)(t/0.1)%(text.length()+1));

        font(text.substr(0,length)).drawAt(center.movedBy(4,4), Palette::Black);
        font(text.substr(0,length)).drawAt(center,Palette::Red);
    }
}