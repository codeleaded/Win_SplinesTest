#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Splines.h"

CRSpline path;
BCurve curve;
Vec2* pSelected = NULL;
float Ship = 0.0f;

void Setup(AlxWindow* w){
    path = (CRSpline){ Vector_New(sizeof(Vec2)) };
    for(int i = 0;i<20;i++){
        Vector_Push(&path.points,(Vec2[]){(Vec2){i*10,i*5}});
    }

    curve = (BCurve){ { 300.0f,300.0f },{ 600.0f,300.0f },{ 500.0f,500.0f } };
}

void Update(AlxWindow* w){
    if(Stroke(ALX_MOUSE_L).PRESSED){
        pSelected = NULL;
        for(int i = 0;i<path.points.size;i++){
            Vec2* point = (Vec2*)Vector_Get(&path.points,i);
            float dx = point->x-GetMouse().x;
            float dy = point->y-GetMouse().y;
            if(sqrtf(dx*dx + dy*dy)<5.0f){
                pSelected = point;
            }
        }
        
        float dx = curve.p1.x-GetMouse().x;
        float dy = curve.p1.y-GetMouse().y;
        if(sqrtf(dx*dx + dy*dy)<5.0f){
            pSelected = &curve.p1;
        }
        dx = curve.p2.x-GetMouse().x;
        dy = curve.p2.y-GetMouse().y;
        if(sqrtf(dx*dx + dy*dy)<5.0f){
            pSelected = &curve.p2;
        }
        dx = curve.p3.x-GetMouse().x;
        dy = curve.p3.y-GetMouse().y;
        if(sqrtf(dx*dx + dy*dy)<5.0f){
            pSelected = &curve.p3;
        }
    }
    if(Stroke(ALX_MOUSE_L).DOWN){
        if(pSelected){
            pSelected->x = GetMouse().x;
            pSelected->y = GetMouse().y;
        }
    }
    
    Clear(BLACK);

    //for(float t = 0.0f;t<(float)path.points.size-3.0f;t+=0.005f){
    //    Vec2 pos = CRSpline_Point(&path,t,FALSE);
    //    Draw(pos.x,pos.y,WHITE);
    //}

    CRSpline_Render(&path,RED,1);

    BCurve_Render(curve,BLUE);

    if(pSelected){
        RenderRect(pSelected->x-1,pSelected->y-1,3,3,YELLOW);
    }

    Vec2 pos = CRSpline_Point(&path,Ship,1);
    Vec2 dir = CRSpline_Gradient(&path,Ship,1);
    if(dir.x!=0.0f){
        float a = atan2f(-dir.y,dir.x);
        Vec2 p1 = { sinf(a), cosf(a)};
        Vec2 p2 = {-sinf(a),-cosf(a)};
        p1 = Vec2_Mulf(p1,10.0f);
        p2 = Vec2_Mulf(p2,10.0f);
        p1 = Vec2_Add(p1,pos);
        p2 = Vec2_Add(p2,pos);
        RenderLine(p1,p2,BLUE,1.0f);
    }

    pos = BCurve_Point(curve,Ship - (int)Ship);
    dir = BCurve_Gradient(curve,Ship - (int)Ship);
    if(dir.x!=0.0f){
        float a = atan2f(-dir.y,dir.x);
        Vec2 p1 = { sinf(a), cosf(a)};
        Vec2 p2 = {-sinf(a),-cosf(a)};
        p1 = Vec2_Mulf(p1,50.0f);
        p2 = Vec2_Mulf(p2,50.0f);
        p1 = Vec2_Add(p1,pos);
        p2 = Vec2_Add(p2,pos);
        RenderLine(p1,p2,BLUE,1.0f);
    }

    Ship += 0.1f * w->ElapsedTime;
    Ship = Ship>path.points.size?0.0f:Ship;
}

void Delete(AlxWindow* w){
    Vector_Free(&path.points);
}

int main(){
    if(Create("Splines",800,600,2,2,Setup,Update,Delete))
        Start();
    return 0;
}