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

    C_List(Object *owner, const std::vector<T> &items, GetStringFunc getString, uint8_t width, uint8_t height) : C_Drawable(width, height), Component(owner), canvas(width, height), items(items), getString(getString), selectedIndex(0), previousIndex(0)
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
        canvas.setTextWrap(false);
        marqueeOffset = 0;
        marqueeTimer = 0;
        marqueeStartDelay = 600;
        marqueeSpeed = 50;      // Delay before the start of the scroll, in milliseconds
        marqueeEndDelay = 1000; // Delay after the end of the scroll, in milliseconds
    };
    void Update(uint8_t deltaTime) override
    {
        // If the marquee has scrolled past the end of the text, start the delay
        std::string selected_item = getString(items[selectedIndex]);
        String text = String(selected_item.c_str());

        if (this->canvas.getTextWidth(text) > 58)
        {
            if (marqueeOffset > (int)this->canvas.getTextWidth(text) - 58)
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
        // Check if selectedIndex has changed
        if (selectedIndex != previousIndex)
        {
            // Reset marqueeOffset
            marqueeOffset = 0;
            marqueeTimer = 0;

            // Update previousIndex
            previousIndex = selectedIndex;
        }

        // Clear the canvas
        this->canvas.fillScreen(16U);

        // Calculate the start index and end index for the items to be displayed
        int start = std::max(0, selectedIndex - 2);
        int end = std::min((int)items.size(), selectedIndex + 3);

        // Calculate the y offset for drawing the items
        int yOffset = (canvas.height() / 2) + 4 - ((selectedIndex - start) * 22);
        // Draw the items
        for (int i = start; i < end; ++i)
        {
            // Set the color based on whether this item is selected
            this->canvas.setTextColor(i == selectedIndex ? 0xffff : 0x4);

            // Draw the item
            if (i == selectedIndex)
            {
                this->canvas.setCursor(0 - marqueeOffset, yOffset + (i - start) * 22);
                std::string text = getString(items[i]);
                this->canvas.print(text.c_str());
            }
            else
            {
                this->canvas.setCursor(0, yOffset + (i - start) * 22);
                std::string substring = getString(items[i]).substr(0, 12);
                this->canvas.print(substring.c_str());
            }
        }

        canvas.drawGrayscaleBitmap(GetOffsetPosition().x, GetOffsetPosition().y, this->canvas.getBuffer(), 16U, 64, 127);
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

    int selectedIndex, previousIndex;

    int marqueeOffset, marqueeTimer, marqueeSpeed, marqueeStartDelay, marqueeEndDelay = 0;

    GFXcanvas8 canvas;
};

#endif// C_LIST_HPP
