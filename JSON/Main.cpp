# include <Siv3D.hpp>

struct Item
{
    // アイテムのラベル
    String label;

    // アイテムの左上座標
    Point pos;
};

// 再帰的に JSON の要素を表示
void ShowObject(const JSONValue& value)
{
    for (const auto& object : value.objectView())
    {
        switch (object.value.getType())
        {
        case JSONValueType::Empty:
            Print << U"[Empty]" << object.name;
            break;
        case JSONValueType::Null:
            Print << U"[Null]" << object.name;
            break;
        case JSONValueType::Object:
            Print << U"[Object]" << object.name;
            ShowObject(object.value);
            break;
        case JSONValueType::Array:
            Print << U"[Array]" << object.name;
            for (const auto& element : object.value.arrayView())
            {
                ShowObject(element);
            }
            break;
        case JSONValueType::String:
            Print << U"[String]" << object.name;
            Print << object.value.getString();
            break;
        case JSONValueType::Number:
            Print << U"[Number]" << object.name;
            Print << object.value.get<double>();
            break;
        case JSONValueType::Bool:
            Print << U"[Bool]" << object.name;
            Print << object.value.get<bool>();
            break;
        }
    }
}

void Main()
{
    // JSON ファイルからデータを読み込む
    const JSONReader json(U"example/config/config.json");

    if (!json) // もし読み込みに失敗したら
    {
        throw Error(U"Failed to load `config.json`");
    }

    // JSON データをすべて表示
    ShowObject(json);

    Print << U"-----";

    // 要素のパスで値を取得
    const String windowTitle = json[U"Window.title"].getString();
    const int32 windowWidth = json[U"Window.width"].get<int32>();
    const int32 windowHeight = json[U"Window.height"].get<int32>();
    const bool windowSizable = json[U"Window.sizable"].get<bool>();
    const ColorF sceneBackground = json[U"Scene.background"].get<ColorF>();

    Window::SetTitle(windowTitle);
    Window::Resize(windowWidth, windowHeight);
    Window::SetStyle(windowSizable ? WindowStyle::Sizable : WindowStyle::Fixed);
    Scene::SetBackground(sceneBackground);

    // 数値の配列を JSON データから作成
    Array<int32> values;
    {
        for (const auto& object : json[U"Array.values"].arrayView())
        {
            values << object.get<int32>();
        }
    }
    Print << values;

    // アイテムの配列を JSON データから作成
    Array<Item> items;
    {
        for (const auto& object : json[U"Items"].arrayView())
        {
            Item item;
            item.label = object[U"label"].getString();
            item.pos = Point(object[U"pos.x"].get<int32>(), object[U"pos.y"].get<int32>());
            items << item;
        }
    }

    // アイテム描画用のフォント
    const Font font(30, Typeface::Bold);

    while (System::Update())
    {
        // アイテムを描画
        for (const auto& item : items)
        {
            const Rect rect(item.pos, 180, 80);

            rect.draw();

            font(item.label).drawAt(rect.center(), ColorF(0.25));
        }
    }
}