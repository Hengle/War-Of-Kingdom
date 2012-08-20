/*
	config�ܹ�������ͼ��ÿ������lParamֵ����������
	1)�������config: ����config��config������ֵ��Ҫ������ֵ��game_config_�ж�λ�и�config��
	2)�����attribute����attribute��attributeλ��ֵ������ǰû��ʹ�ã�ֻ���������ˣ�
*/

#include "global.hpp"
#include "game_config.hpp"
#include "config.hpp"
#include "foreach.hpp"
#include "loadscreen.hpp"
#include "stdafx.h"
#include <windowsx.h>
#include <string.h>
#include <shlobj.h> // SHBrowseForFolder

#include "resource.h"

#include "xfunc.h"
#include "win32x.h"
#include "struct.h"

void cfg_refresh(HWND hctl);
void cfg_2_tv(HWND hctl, HTREEITEM htvroot, const config& cfg, uint32_t maxdeep);

class visual_cfg
{
public:
	visual_cfg();

// attribute member
	HTREEITEM htvroot_;
	config game_config_;
};

visual_cfg::visual_cfg()
{}

visual_cfg vcfg;

void cfg_enter_ui(void)
{
	HWND hctl = GetDlgItem(gdmgr._hdlg_cfg, IDC_TV_CFG_EXPLORER);

	StatusBar_Idle();

	strcpy(gdmgr.cfg_fname_, gdmgr._menu_text);
	StatusBar_SetText(gdmgr._hwnd_status, 0, gdmgr.cfg_fname_);

	cfg_refresh(hctl);
	return;
}

BOOL cfg_hide_ui(void)
{
	return TRUE;
}

// �Ի�����Ϣ������
BOOL On_DlgCfgInitDialog(HWND hdlgP, HWND hwndFocus, LPARAM lParam)
{
	HWND hctl = GetDlgItem(hdlgP, IDC_TV_CFG_EXPLORER);

	gdmgr._hdlg_cfg = hdlgP;

	TreeView_SetImageList(hctl, gdmgr._himl, TVSIL_NORMAL);

	return FALSE;
}

void On_DlgCfgCommand(HWND hdlgP, int id, HWND hwndCtrl, UINT codeNotify)
{
/*	switch(id) {
	case IDC_BT_ABOUT_HELP:
		break;
	default:
		break;
	}
*/
	return;
}

BOOL On_DlgCfgNotify(HWND hdlgP, int DlgItem, LPNMHDR lpNMHdr)
{
	LPNMTREEVIEW			lpnmitem;
	HTREEITEM				htvi;
	TVITEMEX				tvi;
	POINT					point;
	
	if (lpNMHdr->idFrom != IDC_TV_CFG_EXPLORER) {
		return FALSE;
	}

	lpnmitem = (LPNMTREEVIEW)lpNMHdr;

	// NM_RCLICK/NM_CLICK/NM_DBLCLK��Щ֪ͨ��������,�丽������û��ָ�����ĸ�Ҷ�Ӿ��,
	// ��ͨ���ж�����������ж����ĸ�Ҷ�ӱ�����?
	// 1. GetCursorPos, �õ���Ļ����ϵ�µ��������
	// 2. TreeView_HitTest1(��д��),����Ļ����ϵ�µ�������귵��ָ���Ҷ�Ӿ��
	GetCursorPos(&point);	// �õ�������Ļ����
	TreeView_HitTest1(lpNMHdr->hwndFrom, point, htvi);
	
	// NM_��ʾ��ͨ�ÿؼ���ͨ��,��Χ��(0, 99)
	// TVN_��ʾֻ��TreeViewͨ��,��Χ��(400, 499)
	if (lpNMHdr->code == NM_RCLICK) {
	
	} else if (lpNMHdr->code == NM_CLICK) {
		//
		// �������: ������±�������Ŀ¼,��û�����ɹ�Ҷ��,����Ҷ��
		//
		TreeView_GetItem1(lpNMHdr->hwndFrom, htvi, &tvi, TVIF_CHILDREN, NULL);
		if (tvi.cChildren && !TreeView_GetChild(lpNMHdr->hwndFrom, htvi)) {
			std::vector<std::pair<LPARAM, std::string> > vec;
			LPARAM cfg_index;
			const config* cfg = &vcfg.game_config_;

			TreeView_FormVector(lpNMHdr->hwndFrom, htvi, vec);
			for (std::vector<std::pair<LPARAM, std::string> >::reverse_iterator ritor = vec.rbegin(); ritor != vec.rend(); ++ ritor) {
				if (ritor == vec.rbegin()) {
					continue;
				}
				cfg_index = 0;
				foreach (const config::any_child& value, cfg->all_children_range()) {
					if (cfg_index ++ == ritor->first) {
						cfg = &value.cfg;
						break;
					}
				}
			}
			cfg_2_tv(lpNMHdr->hwndFrom, htvi, *cfg, 0);
		}

	} else if (lpNMHdr->code == NM_DBLCLK) {
		//
		// ��config: չ��/�۵�Ҷ��(ϵͳ�Զ�)
		// attribute: �޶���
		//
		
	} else if (lpNMHdr->code == TVN_ITEMEXPANDED) {
		//
		// ��Ҷ���ѱ�չ�����۵�, �۵�ʱ��ɾ��������Ҷ��, (��ΪҪ������config�ڴ˴��ǲ�����)
		//
		if (lpnmitem->action & TVE_COLLAPSE) {
			// TreeView_Walk(lpNMHdr->hwndFrom, lpnmitem->itemNew.hItem, FALSE, NULL, NULL, TRUE);
			// TreeView_SetChilerenByPath(lpNMHdr->hwndFrom, htvi, TreeView_FormPath(lpNMHdr->hwndFrom, htvi, dirname(game_config::path.c_str())));
		}
	}

    return FALSE;
}

BOOL CALLBACK DlgCfgProc(HWND hdlgP, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) {
	case WM_INITDIALOG:
		return On_DlgCfgInitDialog(hdlgP, (HWND)(wParam), lParam);
	HANDLE_MSG(hdlgP, WM_COMMAND, On_DlgCfgCommand);
	HANDLE_MSG(hdlgP, WM_NOTIFY,  On_DlgCfgNotify);
	}
	
	return FALSE;
}

typedef struct {
	HWND		hctl;
	uint32_t	maxdeep;	// �����չmaxdeep��, 0: ֻ��չ��Ŀ¼, 1: ��չ����Ŀ¼�µ�Ŀ¼Ϊֹ, -1: ��Ϊ���޷�����,��ζ��ȫ��չ	
} tv_walk_cfg_param_t;

void walk_cfg_recursion(const config& cfg, HTREEITEM htvroot, char* text, uint16_t deep, tv_walk_cfg_param_t* wcp)
{
	HTREEITEM htvi, htvi_attribute;
	size_t attribute_index, cfg_index = 0;
	
	// attribute
	attribute_index = 0;
	foreach (const config::attribute &istrmap, cfg.attribute_range()) {
		htvi_attribute = TreeView_AddLeaf(wcp->hctl, htvroot);
		sprintf(text, "%s=%s", istrmap.first.c_str(), istrmap.second.str().c_str());
		TreeView_SetItem2(wcp->hctl, htvi_attribute, TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_CHILDREN, attribute_index ++, gdmgr._iico_txt, gdmgr._iico_txt, 0, text);
	}
	// recursively resolve children
	foreach (const config::any_child& value, cfg.all_children_range()) {
		// key
		htvi = TreeView_AddLeaf(wcp->hctl, htvroot);
		strcpy(text, value.key.c_str());
		// ö�ٵ���Ϊֹ,�˸�configһ���к���,ǿ���ó���ǰ��+����
		TreeView_SetItem2(wcp->hctl, htvi, TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_CHILDREN, cfg_index ++, gdmgr._iico_dir, gdmgr._iico_dir, 1, text);
		if (wcp->maxdeep >= deep) {
			walk_cfg_recursion(value.cfg, htvi, text, deep + 1, wcp);
		}
	}
}

void cfg_2_tv(HWND hctl, HTREEITEM htvroot, const config& cfg, uint32_t maxdeep)
{
	char* text;
	tv_walk_cfg_param_t wcp;

	text = (char*)malloc(CONSTANT_1M);

	wcp.hctl = hctl;
	wcp.maxdeep = maxdeep;
	// �˴���subfolders������Ϊ1. ����Ϊ1,����ö�ٳ�����Ŀ¼/�ļ����ᱻ��ӵ�IVI_ROOT,���ڻص�������������,
	// ������������ܻ��ϵͳ����
	walk_cfg_recursion(cfg, htvroot, text, 0, &wcp);

	free(text);
	
	return;
}

// ����ָʾconfigˢ��������ͼ�ؼ�
void cfg_refresh(HWND hctl)
{
	char text[_MAX_PATH];

	wml_config_from_file(std::string(gdmgr.cfg_fname_), vcfg.game_config_);

	// 1. ɾ��Treeview��������
	TreeView_DeleteAllItems(hctl);

	// 2. ��TreeView���һ������
	vcfg.htvroot_ = TreeView_AddLeaf(hctl, TVI_ROOT);
	strcpy(text, basename(gdmgr.cfg_fname_));
	// ����һ��Ҫ��TVIF_CHILDREN, ��������۵����жϳ���cChildrenΪ0, �ٲ���չ��
	TreeView_SetItem1(hctl, vcfg.htvroot_, TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN, 0, 0, 0, 
		vcfg.game_config_.empty()? 0: 1, text);
	cfg_2_tv(hctl, vcfg.htvroot_, vcfg.game_config_, 0);
}
