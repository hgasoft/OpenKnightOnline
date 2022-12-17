// N3UIDef.h: 
//
//////////////////////////////////////////////////////////////////////

#if !defined __N3UIDEF_H__
#define __N3UIDEF_H__

#include <string>

#define DIR_UITEXTURE		("Texture\\UI\\")

const float UI_DEFAULT_Z = 0.9f;
const float UI_DEFAULT_RHW = 1.0f;

// type
enum eUI_TYPE	{	UI_TYPE_BASE = 0,		// none
					UI_TYPE_BUTTON,			// button
					UI_TYPE_STATIC,			// static (���׸��� ���ڰ� ������ Ŭ����)
					UI_TYPE_PROGRESS,		// progress
					UI_TYPE_IMAGE,			// image
					UI_TYPE_SCROLLBAR,		// scroll bar
					UI_TYPE_STRING,			// string
					UI_TYPE_TRACKBAR,		// track bar
					UI_TYPE_EDIT,			// edit
					
					//cerberus 01,12,29
					UI_TYPE_AREA,			// area
					UI_TYPE_TOOLTIP,		// tooltip

					// ecli666
					UI_TYPE_ICON,			// icon
					UI_TYPE_ICON_MANAGER,	// icon manager.. 

					// repent ����
					UI_TYPE_ICONSLOT,		// icon slot
					UI_TYPE_LIST,			// Text List...
				};

// State
enum eUI_STATE	{	UI_STATE_COMMON_NONE = 0,				// �ƹ������� ���� �׳� ����� ���� Ȥ�� �������� ���� �����찡 �������� �������� ���� ����
					UI_STATE_COMMON_MOVE,					// �������� �ϴ� 
					UI_STATE_BUTTON_NORMAL,					// �ƹ������� ���� �׳� ����� ����..
					UI_STATE_BUTTON_DOWN,					// ��ư�� ��������
					UI_STATE_BUTTON_DOWN_2CHECKDOWN,		// ��ư�� �ӽ������� ���� ����(üũ ��ư�� ���, ��¥�� ���� ���°� �ƴϴ�) ������ down���·�
					UI_STATE_BUTTON_DOWN_2CHECKUP,			// ��ư�� �ӽ������� ���� ����(üũ ��ư�� ���, ��¥�� ���� ���°� �ƴϴ�) ������ normal���·�
					UI_STATE_BUTTON_ON,						// ��ư�� ���� ����.. 
					UI_STATE_BUTTON_DISABLE,				// ��ư�� ��Ȱ��ȭ�� ����
					UI_STATE_BUTTON_CLICK,					// ��ư�� ���ȴ� ������ ���� - Click.
					UI_STATE_SCROLLBAR_NULL,				// �ƹ����µ� �ƴ�..
					UI_STATE_SCROLLBAR_TOPBUTTON_DOWN,		// ���� �ø��� ��ư�� ���� ����.
					UI_STATE_SCROLLBAR_BOTTOMBUTTON_DOWN,	// �Ʒ��� ������ ��ư�� ��������.
					UI_STATE_EDTCTRL_ACTIVE,
					UI_STATE_EDTCTRL_UNACTIVE,
					UI_STATE_TRACKBAR_THUMBDRAG,			// Thumb�� �巡�� �ϴ� ���̴�.
					UI_STATE_LIST_ENABLE,					// List ���� ���� ����
					UI_STATE_LIST_DISABLE,					// List ���� ���� �Ұ���

					// ecli666 (�������� ���� ������.. ������ ��ü�� �ƴϴ�..)
//					UI_STATE_ICON_NONE,						// �������� ���� �����찡 �������� �������� ���� ����..
					UI_STATE_ICON_MOVING,					// �������� ���� �����찡 �������� �����̰� �ִ� ����..
//					UI_STATE_ICON_SELECTED,					// �������� ���� �����찡 �������� ������ ����..
					UI_STATE_ICON_WAIT_FROM_SERVER,			// �������� ���� �����찡 �������� �ű�� ������ ���� ������ ��ٸ��� ����..
					UI_STATE_ICON_DO_SUCCESS,				// �������� ���� �����찡 ������ ���� ������ ���� ����..
					UI_STATE_ICON_DO_FAIL,					// �������� ���� �����찡 ������ ���� ���и� ���� ����..
					UI_STATE_ICON_DO_RECOVERY,				// �������� ���� �����찡 ������ ���� ���и� �޾� �����ؾ� �ϴ� ����..
				};

// message
const uint32_t UIMSG_BUTTON_CLICK		= 0x00000001;		// normal ��ư Ŭ��
const uint32_t UIMSG_TRACKBAR_POS		= 0x00000010;		// trackbar�� pos�� �����
const uint32_t UIMSG_SCROLLBAR_POS		= 0x00000100;		// scrollbar�� pos�� �����.
const uint32_t UIMSG_EDIT_RETURN		= 0x00001000;		// Edit���� enter�� ����
const uint32_t UIMSG_EDIT_TAB			= 0x00002000;		// Edit���� Tab�� ����
const uint32_t UIMSG_EDIT_ESCAPE		= 0x00004000;		// Edit���� esc�� ����
const uint32_t UIMSG_ICON_DOWN_FIRST	= 0x00010000;		// Icon�� ���� ���콺 ��ư ó������ �ٿ�..
const uint32_t UIMSG_ICON_DOWN			= 0x00020000;		// Icon�� ���� ���콺 ��ư �ٿ�..
const uint32_t UIMSG_ICON_UP			= 0x00040000;		// Icon�� ���� ���콺 ��ư ��..
const uint32_t UIMSG_ICON_DBLCLK		= 0x00080000;		// Icon�� ��ư ���� Ŭ��
const uint32_t UIMSG_AREA_DOWN_FIRST	= 0x00100000;		// Area�� ���� ���콺 ��ư ó������ �ٿ�..
const uint32_t UIMSG_LIST_SELCHANGE	= 0x00200000;		// List Selection Change..
const uint32_t UIMSG_LIST_DBLCLK		= 0x00400000;
const uint32_t UIMSG_ICON_RDOWN_FIRST	= 0x01000000;		// Icon�� ������ ���콺 ��ư ó������ �ٿ�..
const uint32_t UIMSG_ICON_RDOWN		= 0x02000000;		// Icon�� ������ ���콺 ��ư �ٿ�..
const uint32_t UIMSG_ICON_RUP			= 0x04000000;		// Icon�� ������ ���콺 ��ư ��..
const uint32_t UIMSG_ICON_RDBLCLK		= 0x08000000;		// Icon�� ������ ���콺 ���� Ŭ��
const uint32_t UIMSG_STRING_LCLICK		= 0x10000000;		// string�� ���콺�� ���� Ŭ�� �Ǿ�����...
const uint32_t UIMSG_STRING_LDCLICK	= 0x20000000;		// string�� ���콺�� ���� ���� Ŭ�� �Ǿ�����...

// ����Ʈ�� �޼���
const uint32_t UIMSG_ICONSLOT_LBCLICK	= 0x00010000;		// icon slot�� ���� ���콺 ��ư Ŭ��
const uint32_t UIMSG_ICONSLOT_RBCLICK	= 0x00020000;		// icon slot�� ������ ���콺 ��ư Ŭ��
const uint32_t UIMSG_ICONSLOT_MBCLICK	= 0x00040000;		// icon slot�� ���� ���콺 ��ư Ŭ��
const uint32_t UIMSG_ICONSLOT_MOUSEOVER= 0x00080000;		// icon slot�� ���콺�� ���� �ִ�.

// mouse flag (localinput.h�� ���� ��ġ�ؾ� �Ѵ�.)
const uint32_t UI_MOUSE_LBCLICK	= 0x00000001;
const uint32_t UI_MOUSE_LBCLICKED	= 0x00000002;
const uint32_t UI_MOUSE_LBDOWN		= 0x00000004;
const uint32_t UI_MOUSE_MBCLICK	= 0x00000008;
const uint32_t UI_MOUSE_MBCLICKED	= 0x00000010;
const uint32_t UI_MOUSE_MBDOWN		= 0x00000020;
const uint32_t UI_MOUSE_RBCLICK	= 0x00000040;
const uint32_t UI_MOUSE_RBCLICKED	= 0x00000080;
const uint32_t UI_MOUSE_RBDOWN		= 0x00000100;
const uint32_t UI_MOUSE_LBDBLCLK	= 0x00000200;
const uint32_t UI_MOUSE_MBDBLCLK	= 0x00000400;
const uint32_t UI_MOUSE_RBDBLCLK	= 0x00000800;

// mouse procedure return value flag
const uint32_t UI_MOUSEPROC_NONE			= 0x00000000;	// �ƹ� �ϵ� ���� �ʾҴ�.
const uint32_t UI_MOUSEPROC_DONESOMETHING	= 0x00000001;	// �հ� ���� �ߴ�.
const uint32_t UI_MOUSEPROC_CHILDDONESOMETHING	= 0x00000002;	// �ڽ��� �հ� ���� �ߴ�.(�� �÷��װ� �����Ǿ� ������ �׻� UI_MOUSEPROC_DONESOMETHING�� �����Ǿ��ִ�.)
const uint32_t UI_MOUSEPROC_INREGION		= 0x00000004;	// ���� �ȿ� ���콺 �����Ͱ� �ִ�.
const uint32_t UI_MOUSEPROC_PREVINREGION	= 0x00000008;	// ���� �ȿ� ���� ƽ�� ���콺 �����Ͱ� �־���.
const uint32_t UI_MOUSEPROC_DIALOGFOCUS	= 0x00000010;	// dialog�� ��Ŀ�� �޾Ҵ�.


// ui_string type
const uint32_t UI_STR_TYPE_LINE			= 0x00000000;	// ��Ʈ�� ���� ���� �̱۶������� ��Ƽ��������..
const uint32_t UI_STR_TYPE_HALIGN			= 0x00000001;	// ��Ʈ�� ��������
const uint32_t UI_STR_TYPE_VALIGN			= 0x00000002;	// ��Ʈ�� ��������

// ui style
const uint32_t UISTYLE_NONE				= 0x00000000;
const uint32_t UISTYLE_ALWAYSTOP			= 0x00000001;	// �׻� �ֻ�����
const uint32_t UISTYLE_MODAL				= 0x00000002;	// modal dialog
const uint32_t UISTYLE_FOCUS_UNABLE		= 0x00000004;	// ��Ŀ���� ������ ���� ������
const uint32_t UISTYLE_SHOW_ME_ALONE		= 0x00000008;	// ���� �ڱ� �ڽŸ� ������ �ִ� �ٸ����� �������Ѵٸ�..
const uint32_t UISTYLE_HIDE_UNABLE			= 0x00000010;	// ������ �ʴ� ������
const uint32_t UISTYLE_USER_MOVE_HIDE		= 0x00000020;	// ������ �����̸� ������ ������
const uint32_t UISTYLE_POS_LEFT			= 0x00000040;	// ���ʿ� �޸� ������
const uint32_t UISTYLE_POS_RIGHT			= 0x00000080;	// �����ʿ� �޸� ������

	// button
const uint32_t UISTYLE_BTN_NORMAL			= 0x00010000;	// �Ϲ� ��ư
const uint32_t UISTYLE_BTN_CHECK			= 0x00020000;	// üũ ��ư(toggle��ư)
	// image
const uint32_t UISTYLE_IMAGE_ANIMATE		= 0x00010000;	// ���ϸ��̼� �Ǵ� �̹����̴�.
	// string
const uint32_t UISTYLE_STRING_MULTILINE	= 0x00000000;	// �����ٷ� ǥ�õȴ�.
const uint32_t UISTYLE_STRING_SINGLELINE	= 0x00100000;	// ���ٷθ� ǥ�õȴ�.
const uint32_t UISTYLE_STRING_ALIGNLEFT	= 0x00200000;	// ���� ����(default)
const uint32_t UISTYLE_STRING_ALIGNRIGHT	= 0x00400000;	// ������ ���� (�����϶���)
const uint32_t UISTYLE_STRING_ALIGNCENTER	= 0x00800000;	// ��� ���� (�����϶���)
const uint32_t UISTYLE_STRING_ALIGNTOP		= 0x01000000;	// ��� ���� (�����϶���, default)
const uint32_t UISTYLE_STRING_ALIGNBOTTOM	= 0x02000000;	// �ϴ� ���� (�����϶���)
const uint32_t UISTYLE_STRING_ALIGNVCENTER	= 0x04000000;	// ������� ���� (�����϶���)
	// edit
const uint32_t UISTYLE_EDIT_PASSWORD		= 0x10000000;	// ��ȣ�� �Է¹޴� edit�̴�.
const uint32_t UISTYLE_EDIT_NUMBERONLY		= 0x20000000;
	// progress
const uint32_t UISTYLE_PROGRESS_LEFT2RIGHT	= 0x10000000;	// ���ʿ��� ���������� ����(default)
const uint32_t UISTYLE_PROGRESS_RIGHT2LEFT	= 0x20000000;	// �����ʿ��� �������� ����
const uint32_t UISTYLE_PROGRESS_TOP2BOTTOM	= 0x40000000;	// ���ʿ��� �Ʒ������� ����
const uint32_t UISTYLE_PROGRESS_BOTTOM2TOP	= 0x80000000;	// �Ʒ��ʿ��� �������� ����
	// TrackBar
const uint32_t UISTYLE_TRACKBAR_HORIZONTAL	= 0x00010000;	// ����(default)
const uint32_t UISTYLE_TRACKBAR_VERTICAL	= 0x00020000;	// ����
	// ScrollBar
const uint32_t UISTYLE_SCROLLBAR_HORIZONTAL= 0x00010000;	// ����(default)
const uint32_t UISTYLE_SCROLLBAR_VERTICAL	= 0x00020000;	// ����

	// Icon type.. ecli666..
const uint32_t UISTYLE_ICON_ITEM			= 0x00000010;	// ������ ������..
const uint32_t UISTYLE_ICON_SKILL			= 0x00000020;	// ��ų ������..
const uint32_t UISTYLE_ICON_CERTIFICATION_NEED		= 0X00000100;	// ������ ���� ������ �ʿ��� ������..
const uint32_t UISTYLE_ICON_CERTIFICATION_NONEED	= 0X00000200;	// ������ ���� ������ ���ʿ��� ������..
const uint32_t UISTYLE_ICON_HIGHLIGHT				= 0x00001000;	// No highlight Icon.. 
const uint32_t UISTYLE_DURABILITY_EXHAUST			= 0x00002000;	// Durability exhausted Icon.. 
const uint32_t UISTYLE_DISABLE_SKILL	= 0x00004000;	// Disable Skill Icon..
const uint32_t UISTYLE_ICON_NO_HIGHLIGHT			= 0x00000000;	// Highlight Icon..

// structures
struct __FLOAT_RECT
{
	float left, top, right, bottom;
	BOOL PtInRect2D(float x, float y) const {return ((x>=left && x<=right && y>=top && y<=bottom)?TRUE:FALSE);}
	BOOL PtInRect3D(float x, float y) const {return ((x>=left && x<=right && y>=bottom && y<=top)?TRUE:FALSE);}
};

#endif // #if !defined __N3UIDEF_H__
