// Cat.cpp : implementation file
//

#include "stdafx.h"
#include "hook.h"
#include "Cat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define CATCOLOR RGB(150, 150, 150)
#define IDT_RELEASEMSG 1

static UINT UWM_GIVE_HINT = ::RegisterWindowMessage(UWM_GIVE_HINT_MSG);
/////////////////////////////////////////////////////////////////////////////
// CCat

CCat::CCat()
{
 active = FALSE;
 smiling = FALSE;
 pawleft = FALSE;
 pawright = FALSE;
}

CCat::~CCat()
{
}


BEGIN_MESSAGE_MAP(CCat, CStatic)
	//{{AFX_MSG_MAP(CCat)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat message handlers

/****************************************************************************
*                                CCat::whisker
* Inputs:
*       CDC & dc: DC to draw in
*	CWhisker w: Whisker descriptor
*	int offset: offset of this whisker from whisker baseline
* Result: void
*       
* Effect: 
*       Draws the whiskers symmetrically left and right
****************************************************************************/

void CCat::whisker(CDC & dc, CWhisker w, int offset)
   {
    dc.MoveTo(w.base.x + w.startdx,            w.base.y + w.deltay * offset);
    dc.LineTo(w.base.x + w.startdx + w.length, w.base.y + w.deltay * offset);
    dc.MoveTo(w.base.x - w.startdx,            w.base.y + w.deltay * offset);
    dc.LineTo(w.base.x - w.startdx - w.length, w.base.y + w.deltay * offset);
   } // CCat::whisker

/****************************************************************************
*                                  CCat::eye
* Inputs:
*       CDC & dc: DC
*	CPoint pt: Point for eye
*	int size: Diameter of eye
*	CPoint see: Point we are looking at
* Result: void
*       
* Effect: 
*       Draws the eye. Draws the eyeball if active.
****************************************************************************/

void CCat::eye(CDC & dc, CPoint pt, int size, CPoint see)
    {
     CBrush eyebrush(RGB(255, 255, 255));
     CBrush eyeballbrush(RGB(0, 0, 0));
     int save = dc.SaveDC();
     if(active)
	{ /* draw active eye */
	 // Draw the eye. This is a simple circle
	 dc.SelectObject(&eyebrush);
	 CRect eye(pt.x - size / 2, pt.y - size / 2, pt.x + size / 2, pt.y + size / 2);
	 dc.Ellipse(eye);

	 // We need to convert the center of the eye to screen coordinates
	 // so we can compute the angle to the see point, which is in
	 // screen coordinates
	 CPoint spt = pt;
	 ClientToScreen(&spt);
	 int dx = see.x - spt.x;
	 int dy = see.y - spt.y;

	 // Simple geometry...compute the position of the eyeball
#define EYEBALL_RATIO 5
	 double h = sqrt((double)(dx * dx) + (double)(dy * dy));
	 double ratio = ((double)size / 2.0 - (double)size / (double)EYEBALL_RATIO) / h;
	 CPoint eb(pt.x + (int)((double)dx * ratio), pt.y + (int)((double)dy * ratio));

	 // Now draw the eyeball
	 dc.SelectObject(&eyeballbrush);
	 CRect eyeball(eb.x - size / EYEBALL_RATIO,
		       eb.y - size / EYEBALL_RATIO,
		       eb.x + size / EYEBALL_RATIO,
		       eb.y + size / EYEBALL_RATIO);
	 dc.Ellipse(eyeball);
	} /* draw active eye */
     else
	{ /* draw sleeping eye */
	 // The cat is aslepp, draw the eye closed
	 dc.MoveTo(pt.x - size / 2, pt.y);
	 dc.LineTo(pt.x + size / 2, pt.y);
	} /* draw sleeping eye */
     dc.RestoreDC(save);
    } // CCat::eye

/****************************************************************************
*                              CCat::GetEyeParam
* Inputs:
*       CPoint & eyept: Coordinate to be set for center of eye
*	int & eyewidth: Diameter of eye
*	BOOL left: TRUE for left eye, FALSE for right eye
* Result: void
*       
* Effect: 
*       Computes the center point of the eye
* Notes:
*	The various numeric parameters were chosen empirically to make
*	the eyes look nice on the face.
****************************************************************************/

void CCat::GetEyeParam(CPoint & eyept, int & eyewidth, BOOL left)
    {
     CRect r;
     GetClientRect(&r);
     CPoint center = GetFaceCenter();
     int dx = r.Width() / 11;
     eyept.x = center.x + (left ? -dx : dx);
     eyept.y = center.y = center.y - r.Height() / 9;
     eyewidth = r.Width() / 13;
    } // CCat::GetEyeParam

/****************************************************************************
*                             CCat::GetFaceCenter
* Result: CPoint
*       The center of the face
* Notes:
*	The various numeric parameters (other than "/ 2") are empirically
*	derived to make the face look nice.
****************************************************************************/

CPoint CCat::GetFaceCenter()
    {
     CRect r;
     GetClientRect(&r);
     return CPoint(r.Width()/2,
		   r.Height() / 2 + 2 * r.Height() / 10);
    } // CCat::GetFaceCenter

/****************************************************************************
*                            CCat::InvalidateEyes
* Result: void
*       
* Effect: 
*       Invalidates the eyes
****************************************************************************/

void CCat::InvalidateEyes()
    {
     CPoint eyept;
     int eyewidth;
     CPoint center = GetFaceCenter();
     GetEyeParam(eyept, eyewidth, TRUE);
     CRect L(eyept.x - eyewidth / 2, eyept.y - eyewidth / 2,
	     eyept.x + eyewidth / 2, eyept.y + eyewidth / 2);
     InvalidateRect(L);
     GetEyeParam(eyept, eyewidth, FALSE);
     CRect R(eyept.x - eyewidth / 2, eyept.y - eyewidth / 2,
	     eyept.x + eyewidth / 2, eyept.y + eyewidth / 2);
     InvalidateRect(R);
    } // CCat::InvalidateEyes

/****************************************************************************
*                             CCat::GetMouthParam
* Inputs:
*       CRect & mr:
*	int & mouthwidth:
* Result: void
*       
* Effect: 
*       Computes the mouth parameters
* Notes:
*	The various numeric parameters (other than "/ 2") are empirically
*	derived to make the face look nice.
****************************************************************************/

void CCat::GetMouthParam(CRect & mr)
    {
     CPoint center = GetFaceCenter();
     CRect r;
     GetClientRect(&r);
     int mw = r.Width() / 5;
     mr.left = center.x - mw / 2;
     mr.bottom = center.y + r.Width() / 9;
     mr.right = mr.left + mw;
     mr.top = mr.bottom - mw / 8;
    } // CCat::GetMouthParam

/****************************************************************************
*                                 CCat::mouth
* Inputs:
*       CDC & dc: DC to use
* Result: void
*       
* Effect: 
*       Draws the mouth. It is either a smiling mouth or a flat mouth
****************************************************************************/

void CCat::mouth(CDC & dc)
    {
     CRect mr;
     GetMouthParam(mr);
     dc.FillSolidRect(&mr, CATCOLOR);

     if(smiling)
	{ /* smiling mouth */
	 dc.MoveTo(mr.left, mr.top);
	 dc.LineTo(mr.left + mr.Width() / 6, mr.bottom);
	 dc.LineTo(mr.right - mr.Width() / 6, mr.bottom);
	 dc.LineTo(mr.right + 1, mr.top + 1); // +1 to get endpoint visible
	} /* smiling mouth */
     else
	{ /* straight */
	 dc.MoveTo(mr.left, mr.bottom);
	 dc.LineTo(mr.right, mr.bottom);
	} /* straight */
    } // CCat::mouth

/****************************************************************************
*                              CCat::GetEarParam
* Inputs:
*	CPoint pt[3]: The three endpoints of the triangle that forms the ear
*       BOOL left: TRUE for left ear, FALSE for right ear
* Result: void
*       
* Effect: 
*       Fills in the pt structure with the endpoints of the ears
****************************************************************************/

void CCat::GetEarParam(CPoint pt[3], BOOL left)
    {
     CRect face = GetFaceRect();
     CPoint center = GetFaceCenter();
#define EARFACTOR 4
     pt[0].x = center.x + (left ? -face.Width() : face.Width()) / EARFACTOR;
     pt[0].y = center.y;
     pt[1].x = center.x + (left ? -face.Width() : face.Width()) / EARFACTOR;
     pt[1].y = face.top - face.Height() / 7;
     pt[2].x = center.x;
     pt[2].y = center.y;
    } // CCat::GetEarParam

/****************************************************************************
*                                  CCat::ear
* Inputs:
*       CDC & dc:
*	CPoint pt[3]:
* Result: void
*       
* Effect: 
*       Draws the ear
* Notes:
*	To simplify the placement, we draw two large triangles. They are
*	clipped by the face. To get the clipping region, we create a
*	region that is the entire area, then subtract out the face, so the
*	triangles are nicely clipped. 
****************************************************************************/

void CCat::ear(CDC & dc, CPoint pt[3])
    {
     CBrush br(CATCOLOR);
     CRect face = GetFaceRect();

     // Compute the elliptic region which is the face.
     CRgn faceRgn;
     faceRgn.CreateEllipticRgn(face.left, face.top, face.right, face.bottom);

     // Compute the rectangular region which is the entire area
     CRect r;
     GetClientRect(&r);
     CRgn wholeRgn;
     wholeRgn.CreateRectRgn(r.left, r.top, r.right, r.bottom);

     // Compute a region which is the destination of the
     // intersection computation. It can be empty, since
     // its parameters will be replaced by CombineRgn
     CRgn drawable;
     drawable.CreateRectRgn(0, 0, 0, 0);

     // Now subtract the face from the whole area and store it
     // in the drawable region area
     drawable.CombineRgn(&wholeRgn, &faceRgn, RGN_DIFF);

     int save = dc.SaveDC();

     // Select the region as the clipping region
     dc.SelectObject(&drawable);

     // Select the brush, and draw the ear. Only that part
     // which is outside the face appears
     dc.SelectObject(br);
     dc.Polygon(pt, 3);

     // Restore the DC
     dc.RestoreDC(save);
    } // CCat::ear

/****************************************************************************
*                              CCat::GetPawParam
* Inputs:
*	CRect & pawr: Center of paw
*       BOOL left: TRUE for left, FALSE for right
* Result: void
*       
* Effect: 
*       Computes the enclosing rectangle of the paw
* Notes:
*	The various numeric parameters (other than "/ 2") are empirically
*	derived to make the face look nice.
****************************************************************************/

void CCat::GetPawParam(CRect & pawr, BOOL left)
    {
     CRect face = GetFaceRect();
     int width = face.Width() / 5;
     pawr.left = (left ? face.left : face.right) - width / 2;
     pawr.right = pawr.left + width;
     pawr.bottom = face.bottom; 
     pawr.top = face.bottom - width / 2;
    } // CCat::GetPawParam

/****************************************************************************
*                                  CCat::paw
* Inputs:
*	CDC & dc: 
*       CRect pawr: Paw rectangle
* Result: void
*       
* Effect: 
*       Draws the paw
* Notes:
*             ***********
*            *           *
*           *    |   |    *
*           ***************
****************************************************************************/

void CCat::paw(CDC & dc, CRect pawr, BOOL pawdown)
    {
     if(pawdown)
	{ /* draw paw */
	 int y = pawr.bottom;
	 // To use the Pie function, we have to double the
	 // height of the rectangle because of how
	 // Pie works
	 pawr.bottom += pawr.Height(); // double area
	 dc.Pie(&pawr, CPoint(pawr.right, y),
	              CPoint(pawr.left, y));

	 // Now draw the toes
	 int x = pawr.left + pawr.Width() / 3;
         #define CLAWHEIGHT (pawr.Height() / 4)
	 dc.MoveTo(x, y);
	 dc.LineTo(x, y - CLAWHEIGHT);
	 x = pawr.left + (2 * pawr.Width()) / 3;
	 dc.MoveTo(x, y);
	 dc.LineTo(x, y - CLAWHEIGHT);
	} /* draw paw */
     else
	// Just "erase" the paw by drawing the background color
	dc.FillSolidRect(&pawr, ::GetSysColor(COLOR_3DFACE));
    } // CCat::paw

/****************************************************************************
*                              CCat::GetFaceRect
* Result: CRect
*       Face rectangle
* Notes:
*	The various numeric parameters (other than "/ 2") are empirically
*	derived to make the face look nice.
****************************************************************************/

CRect CCat::GetFaceRect()
    {
     CPoint center = GetFaceCenter();
     CRect r;
     GetClientRect(&r);
     CRect face(center.x - 3 * r.Width() / 10,
		center.y - (r.bottom - center.y),
		center.x + 3 * r.Width() / 10,
		r.bottom - ::GetSystemMetrics(SM_CXBORDER));
     return face;
    } // CCat::GetFaceRect

/****************************************************************************
*                                CCat::OnPaint
* Result: void
*       
* Effect: 
*       Draws the cat
****************************************************************************/

void CCat::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect r;
	GetClientRect(&r);

	//+---------------------------------------------------------------+
	//|                                                               |
	//|                                                               |
	//|                                                               |
	//|                       *                *                      |
	//|                      * *              * *                     |
	//|                   **************************                  |
	//|                  *                          *                 |
	//|                 *                            *                |
	//|                *         O           O        *               |
	//|                *                              *               |
	//|         -------*-----                    -----*-------        |
	//|          ------*-----                    -----*------         |
	//|          ------*-----                    -----*------         |
	//|                 *        *************       *                |
	//|    *****         *                          *     *****       |
	//|   * ... *         **************************     * ... *      |
	//+---------------------------------------------------------------+
	CPoint center = GetFaceCenter();

	CRect face = GetFaceRect();

	CBrush br(CATCOLOR);
	int save = dc.SaveDC();
	 
	dc.SelectObject(&br);
	dc.Ellipse(&face);
	
	CWhisker dwhisker(r.Width() / 4,	// start DX
			  r.Width() / 6,        // length of whisker
			  r.Height() / 15,	// spacing of whiskers
			  center.x,		// center reference
			  center.y);		// vertical center reference
	whisker(dc, dwhisker, 0);
	whisker(dc, dwhisker, 1);
	whisker(dc, dwhisker, 2);

	CPoint see;
	GetCursorPos(&see);

	
	CPoint eyept;
	int eyewidth;
	GetEyeParam(eyept, eyewidth, TRUE);
	eye(dc, eyept, eyewidth, see);
	GetEyeParam(eyept, eyewidth, FALSE);
	eye(dc, eyept, eyewidth, see);

	mouth(dc);

	CPoint earpt[3];
	GetEarParam(earpt, TRUE);
	ear(dc, earpt);
	GetEarParam(earpt, FALSE);
	ear(dc, earpt);

	CRect pawr;
	GetPawParam(pawr, TRUE);
	paw(dc, pawr, pawleft);
	GetPawParam(pawr, FALSE);
	paw(dc, pawr, pawright);

	dc.RestoreDC(save);
	// Do not call CStatic::OnPaint() for painting messages
}

/****************************************************************************
*                             CCat::OnLButtonDown
* Inputs:
*       UINT nFlags:
*	CPoint point: Place where button went down
* Result: void
*       
* Effect: 
*       If a paw has captured the mouse, release it. Redraw the paw so it
*	disappears back behind the wall
* Notes:
*	If we just release the cursor, it will be immediately recaptured
*	because it is still in the paw area. This makes it hard to get
*	the mouse away. So we cause the mouse to "run away" when it is clicked
****************************************************************************/

void CCat::OnLButtonDown(UINT nFlags, CPoint point) 
   {
    if(pawleft || pawright)
       { /* captured */
	::ClipCursor(NULL);
	ReleaseCapture();

	// Force a redraw
	CRect pawr;
	if(pawleft)
	   GetPawParam(pawr, TRUE);
	else
	   GetPawParam(pawr, FALSE);

	smiling = pawleft = pawright = FALSE;
	InvalidateRect(&pawr);

	// Because the cat is no longer smiling, redraw the mouth
	CRect mr;
	GetMouthParam(mr);
	mr.InflateRect(1, 1);
	InvalidateRect(&mr);

	// We are going to move the cursor out of the way. But
	// we force UpdateWindow to redraw so the paw goes away
	UpdateWindow();
	
	ClientToScreen(&pawr);
	int dx = 10;
	int y = pawr.top + pawr.Height() / 2;
	int dy = 5;
	for(UINT i = 0; i < (UINT)(70 / dx); i++)
	   { /* escape */
	    ::SetCursorPos(pawr.right + 5 + i * dx, y + i * dy);
	    ::Sleep(75);
	   } /* escape */
	// Now release the capture
       } /* captured */
	
    KillTimer(IDT_RELEASEMSG);
    ReleaseCapture();
    GetParent()->PostMessage(UWM_GIVE_HINT, FALSE);
    CStatic::OnLButtonDown(nFlags, point);
   }

/****************************************************************************
*                              CCat::OnMouseMove
* Inputs:
*       UINT nFlags: mouse flags
*	CPoint point: Point where mouse is
* Result: void
*       
* Effect: 
*       If the program is active, snabs the mouse. The cat reaches up and
*	grabs it.
****************************************************************************/

void CCat::OnMouseMove(UINT nFlags, CPoint point) 
   {
    if(active)
       { /* grab it? */
        CRect pawr;
	pawr.SetRectEmpty();
	CRect t;
	GetPawParam(t, TRUE);
	if(t.PtInRect(point))
	   { /* hit left */
	    pawleft = TRUE;
	    pawr = t;
	   } /* hit left */
	else
	   { /* try right */
	    GetPawParam(t, FALSE);
	    if(t.PtInRect(point))
	       { /* hit right */
		pawright = TRUE;
		pawr = t;
	       } /* hit right */
	   } /* try right */

	// At this point, we may have detected the mouse is under
	// the cat's paw

	// These parameters limit the mouse movement so it can't move
	// out from under the cat's paw
#define CLIP_LEFT 0
#define CLIP_RIGHT 7 // based on the image of the cursor

	if((pawleft || pawright) && !pawr.IsRectEmpty())
	   { /* lock it in */
	    // Force a redraw of the paw
	    InvalidateRect(&pawr);

	    // Compute a rectangle that limits the
	    // mouse movement. This needs to be
	    // at least 1 unit high or the mouse
	    // looks like it is oscillating
	    pawr.top = pawr.bottom;
	    pawr.bottom = pawr.top + 1;
	    pawr.left += CLIP_LEFT;
	    pawr.right -= CLIP_RIGHT;
	    ClientToScreen(&pawr); // we need screen coordinates
	    ::ClipCursor(&pawr);
	    // By setting capture, we don't get WM_SETCURSOR messages
	    // and there will be no attempt to revert the cursor to
	    // the default shape for the control.
	    SetCapture();

	    // Set up a timing to tell how to release the mouse
	    SetTimer(IDT_RELEASEMSG, 5000, NULL);

	    // The cat is now happy to have captured the mouse
	    // Show a nice cat smile
	    smiling = TRUE;
	    CRect mr;
	    GetMouthParam(mr);
	    mr.InflateRect(1, 1);
	    InvalidateRect(&mr);

	    // Now set the cursor to look like the cat has
	    // really set its paw on it
	    HCURSOR cursor = AfxGetApp()->LoadCursor(IDC_CAPTURED);
	    ::SetCursor(cursor);
	   } /* lock it in */
	} /* grab it? */
    CStatic::OnMouseMove(nFlags, point);
   }

void CCat::OnTimer(UINT nIDEvent) 
   {
    if(nIDEvent == IDT_RELEASEMSG)
       { /* release message */
	GetParent()->PostMessage(UWM_GIVE_HINT, TRUE);
	KillTimer(IDT_RELEASEMSG);
       } /* release message */
	
    CStatic::OnTimer(nIDEvent);
   }
