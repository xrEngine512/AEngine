#pragma once

__interface IExternalHUDElement
{
    void SetCoord(int x, int y);
    void SetScale(float percentScaleX , float percentScaleY );
};