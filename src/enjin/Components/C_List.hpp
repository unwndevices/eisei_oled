#ifndef C_LIST_HPP
#define C_LIST_HPP

#include "enjin/Components/Component.hpp"
#include "enjin/Components/C_Drawable.hpp"
#include "enjin/Components/C_Position.hpp"
#include <Adafruit_GFX.h>
#include <Fonts/VGATypewriter.h>

#include "enjin/Object.hpp"
#include <memory>
#include <iostream>

template <typename T>
class C_List : public Component, public C_Drawable
{
public:
    using GetStringFunc = std::function<std::string(const T &)>;

    C_List(Object *owner, const std::vector<T> &items, GetStringFunc getString) : Component(owner), canvas(64, 127), items(items), getString(getString), selectedIndex(0)
    {
        position = owner->GetComponent<C_Position>();

        if (!position)
        {
            std::cerr << "C_Tooltip requires C_Position component.\n";
        }
    };
    void Awake() override
    {
        // position->SetPosition(Vector2(0, 0));
        canvas.setFont(&VGATypewriter8pt7b);
        canvas.setTextColor(0xffff);
    };
    void Update(uint8_t deltaTime) override{};
    void Draw(GFXcanvas8 &canvas) override
    {
        // Clear the canvas
        this->canvas.fillScreen(7);

        // Calculate the start index and end index for the items to be displayed
        int start = std::max(0, selectedIndex - 2);
        int end = std::min((int)items.size(), selectedIndex + 3);

        // Calculate the y offset for drawing the items
        int yOffset = 8 + (2 - std::min(2, selectedIndex)) * 22;

        // Draw the items
        for (int i = start; i < end; ++i)
        {
            // Set the color based on whether this item is selected
            this->canvas.setTextColor(i == selectedIndex ? 0xffff : 0x4);

            // Draw the item
            this->canvas.setCursor(0, yOffset + (i - start) * 22);
            std::string substring = getString(items[i]).substr(0, 7);
            this->canvas.print(substring.c_str());
        }

        canvas.drawGrayscaleBitmap(position->GetPosition().x, position->GetPosition().y, this->canvas.getBuffer(), 0x7, 64, 127);
    };

    bool ContinueToDraw() const override
    {
        return !owner->IsQueuedForRemoval();
    };
    void SetPosition(Vector2 pos) { position->SetPosition(pos); };

    ///////////////
    void MoveUp()
    {
        if (selectedIndex > 0)
        {
            --selectedIndex;
        }
    }

    void MoveDown()
    {
        if (selectedIndex < items.size() - 1)
        {
            ++selectedIndex;
        }
    }

    T GetCurrentSelection() const
    {
        return items[selectedIndex];
    }

    void SetSelectedIndexByValue(float value, float epsilon = 0.01f)
    {
        for (int i = 0; i < items.size(); ++i)
        {
            if (std::abs(items[i].second - value) < epsilon)
            {
                selectedIndex = i;
                break;
            }
        }
    }

private:
    std::vector<T> items;
    GetStringFunc getString;

    int selectedIndex;

    std::shared_ptr<C_Position> position;

    GFXcanvas8 canvas;
};

#endif // C_LIST_HPP
