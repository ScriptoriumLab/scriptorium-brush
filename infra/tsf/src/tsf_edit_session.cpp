#include "modian/tsf/tsf_edit_session.h"
#include "modian/core/logger/logger_service.h"

namespace modian::brush::infra::tsf {

	STDMETHODIMP tsf_edit_session::DoEditSession(TfEditCookie ec) {
       TF_SELECTION tfSelection;
       ULONG cFetched;

       // 1. 获取当前光标位置
       if (FAILED(context_->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &cFetched)) || cFetched != 1) {
          core::logger_service::logger()->error("DoEditSession: Failed to get selection");
          return E_FAIL;
       }

       // ⚠️ 关键点 1: 克隆 Range
       // 直接操作 selection.range 有时会导致光标行为怪异或 Shift 失败
       ITfRange* pRange = nullptr;
       if (FAILED(tfSelection.range->Clone(&pRange))) {
           core::logger_service::logger()->error("DoEditSession: Failed to clone range");
           tfSelection.range->Release();
           return E_FAIL;
       }

       // 释放原始 selection range，我们只操作 clone
       tfSelection.range->Release();

       // 2. 向前扩展 Range 以覆盖需要删除的字符
       if (backspace_count_ > 0) {
          LONG cchShifted = 0;
          // 尝试向前移动 Start 锚点
          HRESULT hrShift = pRange->ShiftStart(ec, -static_cast<LONG>(backspace_count_), &cchShifted, nullptr);

          if (FAILED(hrShift)) {
              core::logger_service::logger()->error("DoEditSession: ShiftStart failed");
          } else {
              core::logger_service::logger()->info("DoEditSession: ShiftStart requested {}, actual {}", -static_cast<LONG>(backspace_count_), cchShifted);
          }
       }

       // 3. 替换文本 (SetText)
       // 如果 Range 刚才成功扩大了，SetText 会覆盖范围内的内容 (即删除了旧的)
       // 如果 Range 还是空的 (Shift 失败)，SetText 会直接插入
       HRESULT hr = pRange->SetText(ec, 0, text_.data(), static_cast<LONG>(text_.size()));

       if (SUCCEEDED(hr)) {
          // 4. 更新光标位置
          // 将 Range 折叠到末尾 (即新插入文本的后面)
          pRange->Collapse(ec, TF_ANCHOR_END);

          TF_SELECTION sel;
          sel.range = pRange;
          sel.style.ase = TF_AE_NONE;
          sel.style.fInterimChar = FALSE;

          context_->SetSelection(ec, 1, &sel);

          core::logger_service::logger()->info("DoEditSession: backspace '{}'", backspace_count_);
          core::logger_service::logger()->info("DoEditSession: Success. Inserted '{}'", std::string(text_.begin(), text_.end()).c_str()); // 简单的 log，中文可能乱码但能看长度
       } else {
          core::logger_service::logger()->error("DoEditSession: Failed to SetText");
       }

       pRange->Release();
       return hr;
    }
	STDMETHODIMP tsf_edit_session::QueryInterface(const IID& riid, void** ppv_object) {
		if (!ppv_object) return E_POINTER;
		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfEditSession)) {
			*ppv_object = static_cast<ITfEditSession*>(this);
			AddRef();
			return S_OK;
		}
		*ppv_object = nullptr;
		return E_NOINTERFACE;
	}

	STDMETHODIMP_(ULONG) tsf_edit_session::AddRef() {
		return ++ref_count_;
	}

	STDMETHODIMP_(ULONG) tsf_edit_session::Release() {
		const ULONG count = --ref_count_;
		if (count == 0) delete this;
		return count;
	}
}