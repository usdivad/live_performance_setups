//
//  DaalLookAndFeel.cpp
//  dadadaal
//
//  Created by David Su on 12/22/18.
//  Copyright © 2018 usdivad. All rights reserved.
//

#include "DaalLookAndFeel.h"

DaalLookAndFeel::DaalLookAndFeel()
{
    // Window
    setColour(ResizableWindow::backgroundColourId, Colour(0xff292929));
    
    // Slider
    
    // White on brown
    // setColour(Slider::thumbColourId, Colour(0xffffffff));
    // setColour(Slider::rotarySliderFillColourId, Colour(0xffc8a686));
    // setColour(Slider::rotarySliderOutlineColourId, Colour(0xff7b4e2c));
    
    // Brown on brown/grey
    // setColour(Slider::thumbColourId, Colour(0xffc8a686));
    // setColour(Slider::rotarySliderFillColourId, Colour(0xff7b4e2c));
    // setColour(Slider::rotarySliderOutlineColourId, Colour(0xff414141));
    
    // White on brown/grey
    // setColour(Slider::thumbColourId, Colour(0xffffffff));
    // setColour(Slider::rotarySliderFillColourId, Colour(0xff7b4e2c));
    // setColour(Slider::rotarySliderOutlineColourId, Colour(0xff414141));
    
    // White on grey
    setColour(Slider::thumbColourId, Colour(0xffffffff));
    setColour(Slider::rotarySliderFillColourId, Colour(0xff949494));
    setColour(Slider::rotarySliderOutlineColourId, Colour(0xff535353));
    
    setColour(TextButton::buttonColourId, Colour(0xff535353));
    setColour(TextButton::buttonOnColourId, Colour(0xff949494));
    
    setColour(ComboBox::textColourId, Colour(0xffffffff));
    setColour(ComboBox::arrowColourId, Colour(0xffffffff));
    setColour(ComboBox::backgroundColourId, Colour(0xff292929));
    // setColour(ComboBox::buttonColourId, Colour(0xff535353));
    
    setColour(PopupMenu::textColourId, Colour(0xffffffff));
    setColour(PopupMenu::backgroundColourId, Colour(0xff292929));
    setColour(PopupMenu::highlightedBackgroundColourId, Colour(0xff535353));
    
    // Misc
    // setColour(Slider::thumbColourId, Colour(0xff2c597b));
    // setColour(Slider::thumbColourId, Colours::red);
    // setColour(Slider::trackColourId, Colour(0xffc8a686));
    // setColour(Slider::backgroundColourId, Colour(0xff7b4e2c));
    
    
    
    // Text
    setColour(Label::textColourId, Colour(0xffffffff));
}
