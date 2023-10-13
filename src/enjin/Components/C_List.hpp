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
class C_List : public C_Drawable
{
public:
    using GetStringFunc = std::function<std::string(const T &)>;

    C_List(Object *owner, const std::vector<T> &items, GetStringFunc getString, uint8_t width, uint8_t height) : C_Drawable(width, height), Component(owner), items(items), getString(getString), selectedIndex(0), previousIndex(0)
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
        marqueeOffset = 0;
        marqueeTimer = 0;
        marqueeStartDelay = 600;
        marqueeSpeed = 50;      // Delay before the start of the scroll, in milliseconds
        marqueeEndDelay = 1000; // Delay after the end of the scroll, in milliseconds
        maxTextWidth = width - 10;
    };
    void Update(uint8_t deltaTime) override
    {

        if (textWidth > maxTextWidth)
        {
            if (marqueeOffset > (int)textWidth - maxTextWidth)
            {
                marqueeTimer += deltaTime;

                // If the delay has passed, reset the marquee
                if (marqueeTimer > marqueeEndDelay)
                {
                    marqueeOffset = 0;
                    marqueeTimer = 0; // Reset the timer
                }
            }
            // Otherwise, update the marquee offset after the start delay
            else
            {
                marqueeTimer += deltaTime;
                if (!marqueeOffset && marqueeTimer < marqueeStartDelay)
                {
                    return;
                }

                else if (marqueeTimer > marqueeSpeed)
                {
                    marqueeOffset++;
                    marqueeTimer = 0; // Reset the timer
                }
            }
        }
    };

    void Draw(GFXcanvas8 &canvas) override
    {
        // If the marquee has scrolled past the end of the text, start the delay
        std::string selected_item = getString(items[selectedIndex]);
        String text = String(selected_item.c_str());
        textWidth = canvas.getTextWidth(text);
        // Check if selectedIndex has changed
        if (selectedIndex != previousIndex)
        {
            // Reset marqueeOffset
            marqueeOffset = 0;
            marqueeTimer = 0;

            // Update previousIndex
            previousIndex = selectedIndex;
        }

        // Calculate the start index and end index for the items to be displayed
        int start = std::max(0, selectedIndex - 2);
        int end = std::min((int)items.size(), selectedIndex + 3);

        // Calculate the y offset for drawing the items
        int yOffset = (canvas.height() / 2) + 4 - ((selectedIndex - start) * 22);
        // Draw the items
        for (int i = start; i < end; ++i)
        {
            // Set the color based on whether this item is selected
            canvas.setTextColor(i == selectedIndex ? 0xffff : 0x4);

            // Draw the item
            if (i == selectedIndex)
            {
                canvas.setCursor(GetOffsetPosition().x - marqueeOffset, yOffset + (i - start) * 22);
                std::string text = getString(items[i]);
                canvas.print(text.c_str());
            }
            else
            {
                canvas.setCursor(GetOffsetPosition().x, yOffset + (i - start) * 22);
                std::string substring = getString(items[i]).substr(0, 12);
                canvas.print(substring.c_str());
            }
        }
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

    void UpdateItems(const std::vector<T> &newItems)
    {
        items = newItems;
    }

private:
    std::vector<T> items;
    GetStringFunc getString;

    int selectedIndex, previousIndex;

    int marqueeOffset = 0, marqueeTimer = 0, marqueeSpeed = 0, marqueeStartDelay = 0, marqueeEndDelay = 0, textWidth = 0,
        maxTextWidth = 0;
};

#endif // C_LIST_HPP
