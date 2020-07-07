import QtQuick 2.12
import QtWebEngine 1.8

WebEngineView {
    visible: true
    id: webView
    property int actualGame: window.selectedGame
    backgroundColor: "transparent"
    settings.showScrollBars: false
    url: _Config.getGamePresentationUrl(actualGame) //"https://www.youtube.com/embed/T5zxTI1gxOQ?&autohide=1&showinfo=0&amp;fs=0&amp;showinfo=0"
    onContextMenuRequested: function(request) {
        request.accepted = true;
    }
}
