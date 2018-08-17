
var __xamlEngineUrlPrefix = function () {
    var scripts = document.getElementsByTagName('script');
    var thisScriptUrl = scripts[scripts.length - 1].src;
    return thisScriptUrl.substring(0, thisScriptUrl.lastIndexOf("/") + 1);
}();

XamlEngine({
    locateFile: function(filename) {
        return __xamlEngineUrlPrefix + filename;
    }
});