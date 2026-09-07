#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/Splines.h"

TransformedView tv;
CRSpline path;
BCurve curve;
Vec2* pSelected = NULL;

float Ship0 = 0.0f;
float Ship1 = 0.0f;
float Ship0_speed = 0.1f;
float Ship1_speed = 0.1f;

void Setup(AlxWindow* w){
    tv = TransformedView_Make(
        (Vec2){ GetWidth(),GetHeight() },
        (Vec2){ 0.0f,0.0f },
        (Vec2){ 0.01f,0.01f },
        (float)GetWidth() / (float)GetHeight()
    );
    
    path = CRSpline_New();
    for(int i = 0;i<20;i++){
        Vector_Push(&path,(Vec2[]){(Vec2){i*10,i*5}});
    }

    curve = (BCurve){ { 300.0f,300.0f },{ 600.0f,300.0f },{ 500.0f,500.0f } };
}

void Update(AlxWindow* w){
    TransformedView_HandlePanZoom(&tv,w->Strokes,GetMouse());
    const Vec2 m = TransformedView_ScreenWorldPos(&tv,GetMouse());

    if(Stroke(ALX_MOUSE_L).PRESSED){
        pSelected = NULL;
        if(!pSelected) pSelected = CRSpline_Find(&path,m,1.0f);
        if(!pSelected) pSelected = BCurve_Find(&curve,m,1.0f);
    }
    if(Stroke(ALX_MOUSE_L).DOWN){
        if(pSelected){
            pSelected->x = m.x;
            pSelected->y = m.y;
        }
    }

    if(Stroke(ALX_KEY_W).DOWN)      Ship0_speed *= 1.01f;
    if(Stroke(ALX_KEY_S).DOWN)      Ship0_speed *= 0.99f;
    if(Stroke(ALX_KEY_UP).DOWN)     Ship1_speed *= 1.01f;
    if(Stroke(ALX_KEY_DOWN).DOWN)   Ship1_speed *= 0.99f;

    Clear(BLACK);

    CRSpline_Render_CP_TV(WINDOW_STD_ARGS,&tv,&path,1,RED,1.0f,WHITE);
    BCurve_Render_CP_TV(WINDOW_STD_ARGS,&tv,&curve,BLUE,1.0f,WHITE);

    CRSpline_Render_GT_TV(WINDOW_STD_ARGS,&tv,&path,1,Ship0,4.0f,WHITE);
    BCurve_Render_GT_TV(WINDOW_STD_ARGS,&tv,&curve,Ship1,4.0f,WHITE);

    const float len = CRSpline_Length(&path,Ship0,0.0001f,1) / CRSpline_AbsLength(&path,0.0001f,1);
    //const float len = CRSpline_Length(&path,Ship0,0.0001f,1);
    Ship0 += Ship0_speed / len * w->ElapsedTime;
    Ship0 = Ship0>path.size ? 0.0f : Ship0;

    Ship1 += Ship1_speed * w->ElapsedTime;
    Ship1 = Ship1 - (int)Ship1;

    CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0.0f,0.0f,RED,"P: %f | %f",Ship0,Ship1);
    CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0.0f,GetAlxFont()->CharSizeY,RED,"S: %f | %f",Ship0_speed,Ship1_speed);
}

void Delete(AlxWindow* w){
    Vector_Free(&path);
}

int main(){
    if(Create("Splines",1900,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}