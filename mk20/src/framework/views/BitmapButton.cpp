//
// Created by Phillip Schuster on 20.01.16.
//

#include "BitmapButton.h"
#include "../layers/BitmapLayer.h"
#include "../layers/SDBitmapLayer.h"
#include "../core/Application.h"
#include "../animation/Animator.h"
#include "../animation/Animation.h"

BitmapButton::BitmapButton(Rect frame):
View(frame),
_originalFrame(frame),
_touchAnimation(NULL),
_baseLayer(NULL),
_bitmapLayer(NULL)
{
    _delegate = NULL;
    _backgroundColor = Application.getTheme()->getColor(ControlBackgroundColor);
    _color = Application.getTheme()->getColor(ControlTextColor);
    _alternateBackgroundColor = Application.getTheme()->getColor(ControlAlternateBackgroundColor);
    _alternateTextColor = Application.getTheme()->getColor(ControlAlternateTextColor);

    LOG("Creating Bitmap Button");
    LOG_VALUE("Frame",frame.toString());
    LOG_VALUE("This-Frame",_frame.toString());
}


BitmapButton::~BitmapButton() {}

void BitmapButton::createBitmapFrame(Rect* frame, uint16_t width, uint16_t height) {
  //The bitmap is smaller than the frame, create a layer and place the bitmap inside it
  if (frame->width > width || frame->height > height) {
    _baseLayer = new RectangleLayer(*frame);
    _baseLayer->setBackgroundColor(getBackgroundColor());
    _baseLayer->setContext(getContext());

    frame->x += (frame->width - width)/2;
    frame->y += (frame->height - height)/2;
    frame->width = width;
    frame->height = height;

    //Stamp a hole in the base layer where we place the BitmapLayer
    _baseLayer->splitWithRect(*frame);
    addLayer(_baseLayer);
  }
}

void BitmapButton::setBitmap(const uint16_t *bitmapRGB, uint16_t width, uint16_t height) {
  Rect bitmapFrame = _frame;
  createBitmapFrame(&bitmapFrame, width, height);
  _bitmapLayer = new BitmapLayer(bitmapFrame);
  _bitmapLayer->setBitmap(bitmapRGB, width, height);
  _bitmapLayer->setContext(getContext());
  addLayer(_bitmapLayer);
}

void BitmapButton::setBitmap(const uint8_t *bitmap, uint16_t width, uint16_t height) {
  Rect bitmapFrame = _frame;
  createBitmapFrame(&bitmapFrame, width, height);

  _bitmapLayer = new BitmapLayer(bitmapFrame);
  _bitmapLayer->setBitmap(bitmap, width, height);
  _bitmapLayer->setBackgroundColor(getBackgroundColor());
  _bitmapLayer->setColor(getColor());
  _bitmapLayer->setContext(getContext());
  addLayer(_bitmapLayer);
}

void BitmapButton::setBitmap(UIBitmap * bitmap) {
  _sdbitmapLayer = new SDBitmapLayer(_frame);
  _sdbitmapLayer->setBitmap("ui", bitmap->width, bitmap->height, bitmap->offset);
  _sdbitmapLayer->setContext(getContext());
  addLayer(_sdbitmapLayer);
}

void BitmapButton::setFrame(Rect frame)
{
  if (_frame == frame) return;

  View::setFrame(frame);
  _originalFrame = frame;

  _layers.at(0)->setFrame(frame);
}

void BitmapButton::updateButton(ButtonState buttonState)
{
  /*
  if (buttonState == ButtonState::Off)
  {
      if (getBaseLayer() != NULL)
      {
          getBaseLayer()->setBackgroundColor(getBackgroundColor());
      }

      getBitmapLayer()->setBackgroundColor(getBackgroundColor());
      getBitmapLayer()->setColor(getColor());
  }
  else if (buttonState == ButtonState::On)
  {
      if (getBaseLayer() != NULL)
      {
          getBaseLayer()->setBackgroundColor(getAlternateBackgroundColor());
      }

      getBitmapLayer()->setBackgroundColor(getAlternateBackgroundColor());
      getBitmapLayer()->setColor(getAlternateTextColor());
  }
  */
    setNeedsDisplay();
}

bool BitmapButton::touchDown(TS_Point &point)
{
    updateButton(ButtonState::On);

    return true;
}

bool BitmapButton::touchUp(TS_Point &point)
{
    if (_type == ButtonType::Toggle)
    {
        _state = (ButtonState) !_state;
        updateButton(_state);
    }
    else
    {
        updateButton(ButtonState::Off);
    }

    if (_delegate != NULL)
    {
        _delegate->buttonPressed(this);
    }

    return true;
}

void BitmapButton::touchCancelled()
{
    updateButton(ButtonState::Off);
}
