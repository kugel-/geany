import gi

from gi.repository import GObject
from gi.repository import Geany
from gi.repository import GeanyGI

class PythonHelloPlugin2(GObject.Object, Geany.Plugin2):
    __gtype_name__ = 'PythonHelloPlugin2'

    methods = GObject.property(type = int, flags = GObject.PARAM_READABLE, default = Geany.Methods.CLEANUP)
    doc = None

    def do_initialize(self):
        self.doc = GeanyGI.Document.new_file("/tmp/foo.c",
                            GeanyGI.Filetype.get_by_id(Geany.FiletypeID.C), "int")
        self.doc.save(False)

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

def __init__():
    print("foo")
