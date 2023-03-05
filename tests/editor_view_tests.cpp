#include "../src/application.hpp"
#include "../src/windows/editor_view.hpp"

#include <WebView2.h>

#include <catch2/catch_test_macros.hpp>

using namespace live::tritone::vie;

class MockCoreWebView2: public ICoreWebView2
{
public:
    HRESULT QueryInterface(const IID& riid, void** ppvObject) override { return 0; }
    ULONG AddRef() override { return 0; }
    ULONG Release() override { return 0; }
	
    HRESULT STDMETHODCALLTYPE get_Settings(
        ICoreWebView2Settings** settings) override { return 0; }

    HRESULT STDMETHODCALLTYPE get_Source(
        LPWSTR* uri) override { return 0; }

    HRESULT STDMETHODCALLTYPE Navigate(
        LPCWSTR uri) override { return 0; }

    HRESULT STDMETHODCALLTYPE NavigateToString(
        LPCWSTR htmlContent) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_NavigationStarting(
        ICoreWebView2NavigationStartingEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_NavigationStarting(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_ContentLoading(
        ICoreWebView2ContentLoadingEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_ContentLoading(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_SourceChanged(
        ICoreWebView2SourceChangedEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_SourceChanged(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_HistoryChanged(
        ICoreWebView2HistoryChangedEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_HistoryChanged(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_NavigationCompleted(
        ICoreWebView2NavigationCompletedEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_NavigationCompleted(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_FrameNavigationStarting(
        ICoreWebView2NavigationStartingEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_FrameNavigationStarting(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_FrameNavigationCompleted(
        ICoreWebView2NavigationCompletedEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_FrameNavigationCompleted(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_ScriptDialogOpening(
        ICoreWebView2ScriptDialogOpeningEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_ScriptDialogOpening(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_PermissionRequested(
        ICoreWebView2PermissionRequestedEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_PermissionRequested(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_ProcessFailed(
        ICoreWebView2ProcessFailedEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_ProcessFailed(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE AddScriptToExecuteOnDocumentCreated(
        LPCWSTR javaScript,
        ICoreWebView2AddScriptToExecuteOnDocumentCreatedCompletedHandler* handler) override { return 0; }

    HRESULT STDMETHODCALLTYPE RemoveScriptToExecuteOnDocumentCreated(
        LPCWSTR id) override { return 0; }

    HRESULT STDMETHODCALLTYPE ExecuteScript(
        LPCWSTR javaScript,
        ICoreWebView2ExecuteScriptCompletedHandler* handler) override { return 0; }

    HRESULT STDMETHODCALLTYPE CapturePreview(
        COREWEBVIEW2_CAPTURE_PREVIEW_IMAGE_FORMAT imageFormat,
        IStream* imageStream,
        ICoreWebView2CapturePreviewCompletedHandler* handler) override { return 0; }

    HRESULT STDMETHODCALLTYPE Reload(void) override { return 0; }

    HRESULT STDMETHODCALLTYPE PostWebMessageAsJson(
        LPCWSTR webMessageAsJson) override { 
        wcscpy_s(last_message, webMessageAsJson);
        return 0;
    }

    HRESULT STDMETHODCALLTYPE PostWebMessageAsString(
        LPCWSTR webMessageAsString) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_WebMessageReceived(
        ICoreWebView2WebMessageReceivedEventHandler* handler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_WebMessageReceived(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE CallDevToolsProtocolMethod(
        LPCWSTR methodName,
        LPCWSTR parametersAsJson,
        ICoreWebView2CallDevToolsProtocolMethodCompletedHandler* handler) override { return 0; }

    HRESULT STDMETHODCALLTYPE get_BrowserProcessId(
        UINT32* value) override { return 0; }

    HRESULT STDMETHODCALLTYPE get_CanGoBack(
        BOOL* canGoBack) override { return 0; }

    HRESULT STDMETHODCALLTYPE get_CanGoForward(
        BOOL* canGoForward) override { return 0; }

    HRESULT STDMETHODCALLTYPE GoBack(void) override { return 0; }

    HRESULT STDMETHODCALLTYPE GoForward(void) override { return 0; }

    HRESULT STDMETHODCALLTYPE GetDevToolsProtocolEventReceiver(
        LPCWSTR eventName,
        ICoreWebView2DevToolsProtocolEventReceiver** receiver) override { return 0; }

    HRESULT STDMETHODCALLTYPE Stop(void) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_NewWindowRequested(
        ICoreWebView2NewWindowRequestedEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_NewWindowRequested(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_DocumentTitleChanged(
        ICoreWebView2DocumentTitleChangedEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_DocumentTitleChanged(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE get_DocumentTitle(
        LPWSTR* title) override { return 0; }

    HRESULT STDMETHODCALLTYPE AddHostObjectToScript(
        LPCWSTR name,
        VARIANT* object) override { return 0; }

    HRESULT STDMETHODCALLTYPE RemoveHostObjectFromScript(
        LPCWSTR name) override { return 0; }

    HRESULT STDMETHODCALLTYPE OpenDevToolsWindow(void) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_ContainsFullScreenElementChanged(
        ICoreWebView2ContainsFullScreenElementChangedEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_ContainsFullScreenElementChanged(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE get_ContainsFullScreenElement(
        BOOL* containsFullScreenElement) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_WebResourceRequested(
        ICoreWebView2WebResourceRequestedEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_WebResourceRequested(
        EventRegistrationToken token) override { return 0; }

    HRESULT STDMETHODCALLTYPE AddWebResourceRequestedFilter(
        const LPCWSTR uri,
        const COREWEBVIEW2_WEB_RESOURCE_CONTEXT resourceContext) override { return 0; }

    HRESULT STDMETHODCALLTYPE RemoveWebResourceRequestedFilter(
        const LPCWSTR uri,
        const COREWEBVIEW2_WEB_RESOURCE_CONTEXT resourceContext) override { return 0; }

    HRESULT STDMETHODCALLTYPE add_WindowCloseRequested(
        ICoreWebView2WindowCloseRequestedEventHandler* eventHandler,
        EventRegistrationToken* token) override { return 0; }

    HRESULT STDMETHODCALLTYPE remove_WindowCloseRequested(
        EventRegistrationToken token) override { return 0; }

    LPCWSTR get_last_message() {
		return last_message;
    }
	
private:
    wchar_t last_message[1024];
};

SCENARIO("get_projects returns no project when no project exists.", "[editor view]") {	
    live::tritone::vie::editor_view view;
    MockCoreWebView2 mock_core_web_view;

    SECTION("Initialisation") {
        application_.clear();
    }
	
	GIVEN("No project exists") {
		WHEN("on_message_get_projects is called") {
			THEN("an empty response is returned") {				
                view.on_message_received(&mock_core_web_view, L"{\"action\": \"get_projects\"}");
				
                LPCWSTR actual = mock_core_web_view.get_last_message();
                std::wstringstream expected;
                expected << L"{";
                expected << L" \"action\": \"get_projects_callback\",";
                expected << L" \"body\": [";
                expected << L" ]";
                expected << L" }";
				
				REQUIRE(actual == expected.str());
			}
		}
    }
}

SCENARIO("Create a project then get_projects returns a project (created project is saved automatically).", "[editor view]") {
    live::tritone::vie::editor_view view;
    MockCoreWebView2 mock_core_web_view;
	
	SECTION("Initialisation") {
		application_.clear();
	}
	
    GIVEN("No project exists") {		
        WHEN("on_message_new_project is called") {
            THEN("A project id and name is returned") {
                view.on_message_received(&mock_core_web_view, L"{\"action\": \"new_project\"}");

                LPCWSTR actual = mock_core_web_view.get_last_message();
                std::wstringstream expected;
                expected << L"{";
                expected << L" \"action\": \"new_project_callback\",";
                expected << L" \"body\": {";
                expected << L" \"id\": 0,";
                expected << L" \"name\": \"Project 0\"";
                expected << L" }";
                expected << L" }";

                REQUIRE(actual == expected.str());
            }
        }
        WHEN("on_message_get_projects is called") {
            THEN("New project is returned (project created previously is automatically saved)") {
                view.on_message_received(&mock_core_web_view, L"{\"action\": \"get_projects\"}");

                LPCWSTR actual = mock_core_web_view.get_last_message();
                std::wstringstream expected;
                expected << L"{";
                expected << L" \"action\": \"get_projects_callback\",";
                expected << L" \"body\": [";
                expected << L" {";
                expected << L" \"id\": 0,";
                expected << L" \"name\": \"Project 0\",";
                expected << L" \"description\": \"\"";
                expected << L" }";
                expected << L" ]";
                expected << L" }";

                REQUIRE(actual == expected.str());
            }
        }
    }
}

SCENARIO("Create a project, add a module, save project then load it and retrieve project and module.", "[editor view]") {
    live::tritone::vie::editor_view view;
    MockCoreWebView2 mock_core_web_view;

    SECTION("Initialisation") {
        application_.clear();
    }

    GIVEN("No project exists") {
        WHEN("on_message_new_project is called") {
            THEN("A project name is returned") {
                view.on_message_received(&mock_core_web_view, L"{\"action\": \"new_project\"}");

                LPCWSTR actual = mock_core_web_view.get_last_message();
                std::wstringstream expected;
                expected << L"{";
                expected << L" \"action\": \"new_project_callback\",";
                expected << L" \"body\": {";
                expected << L" \"id\": 0,";
                expected << L" \"name\": \"Project 0\"";
                expected << L" }";
                expected << L" }";

                REQUIRE(actual == expected.str());
            }
        }
        WHEN("on_add_module is called") {
            THEN("A module id is returned") {
                view.on_message_received(&mock_core_web_view, L"{\"action\":\"add_module\", \"body\": { \"type\": \"midi-in\", \"position\": { \"x\": 0, \"y\": 0, \"z\": 0 } }}");

                LPCWSTR actual = mock_core_web_view.get_last_message();
                std::wstringstream expected;
                expected << L"{";
                expected << L" \"action\": \"add_module_callback\",";
                expected << L" \"body\": {";
                expected << L" \"module\": { \"id\": 1 }";
                expected << L" }";
                expected << L" }";

                REQUIRE(actual == expected.str());
            }
        }
        WHEN("on_message_export_project is called") {
            THEN("The project is saved to disk.") {
                view.on_message_received(&mock_core_web_view, L"{\"action\":\"export_project\",  \"body\": {\"path\": \"c:/tmp/project0.json\"}}");

                LPCWSTR actual = mock_core_web_view.get_last_message();
                std::wstringstream expected;
                expected << L"{";
                expected << L" \"action\": \"export_project_callback\",";
                expected << L" \"body\": {";
                expected << L" }";
                expected << L" }";

                REQUIRE(actual == expected.str());
            }
        }
        WHEN("on_message_import_project is called") {
            THEN("The project is saved to disk.") {
                view.on_message_received(&mock_core_web_view, L"{\"action\":\"import_project\",  \"body\": {\"path\": \"c:/tmp/project0.json\"}}");

                LPCWSTR actual = mock_core_web_view.get_last_message();
                std::wstringstream expected;
                expected << L"{";
                expected << L" \"action\": \"import_project_callback\",";
                expected << L" \"body\": {";
                expected << L" \"id\": 0,";
                expected << L" \"name\": \"Project 0\",";
                expected << L" \"description\": \"\"";
                expected << L" }";
                expected << L" }";

                REQUIRE(actual == expected.str());
            }
        }
    }
}