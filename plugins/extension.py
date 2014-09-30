import gi

from gi.repository import GObject
from gi.repository import Gtk
from gi.repository import GLib
from gi.repository import Geany
from gi.repository import GeanyGI

class PythonHelloPlugin2(GObject.Object, Geany.Plugin2):
    __gtype_name__ = 'PythonHelloPlugin2'

    methods = GObject.property(type = int, flags = GObject.PARAM_READABLE, default = Geany.Methods.CLEANUP)
    doc = None
    item = None
    keys = None

    def on_clicked(self, action):
        self.doc.editor.indent_type = Geany.IndentType.SPACES

    def on_test_activate(self, key, key_id):
        print("on_test_activate")

    def do_initialize(self):
        self.doc = GeanyGI.Document.new_file("/tmp/foo.c",
                            GeanyGI.Filetype.get_by_id(Geany.FiletypeID.C), "int")
        self.doc.save(False)
        self.item = GeanyGI.UiUtils.image_menu_item_new(Gtk.STOCK_GO_BACK, "foo")
        self.item.connect("activate", self.on_clicked)
        tmp = GeanyGI.UiWidgets.tools_menu().append(self.item)
        self.item.show_all()
        self.keys = GeanyGI.KeyGroup.new("h_p", "Hello Python", 1)
        print(self.keys)
        #~ key = GeanyGI.KeyBinding.new("test", "Test", self.on_test_activate)
        #~ self.keys.add_keybinding(key, 0)
        self.keys.add_keybinding(GeanyGI.KeyBinding.new("test1", "Test1", self.on_test_activate), 0)
        pass

    def do_version_check(self, abi):
        return 218

    def do_configure(self):
        pass

    def do_configure_single(self, widget):
        pass

    def do_help(self):
        pass

    def do_cleanup(self):
        print("cleanup\n")
        self.doc.close()
        self.keys = None
        self.item.destroy()

def __init__():
    print("foo")
