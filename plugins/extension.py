import gi

from gi.repository import GObject
from gi.repository import Peas
from gi.repository import GeanyGObject

class PythonHelloPlugin2(GObject.Object, Peas.Geany):
    __gtype_name__ = 'PythonHelloPlugin2'

    #~ object = GObject.property(type=GObject.Object)

    def do_init(self, data):
        self.doc = GeanyGObject.Document.new_file(None, None, "Hallo");
        print("PythonHelloPlugin.do_init\n")

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

    def do_provides_method(self, which):
        return which == Peas.GeanyFuncs.CLEANUP


class PythonHelloPlugin(GObject.Object, Peas.Activatable):
    __gtype_name__ = 'PythonHelloPlugin'

    object = GObject.property(type=GObject.Object)

    def do_activate(self):
        print("activate")

    def do_deactivate(self):
        print("deactivate")

    def do_update_state(self):
        print("update_state")


#~ if __name__ == "__main__":
	#~ print ("hi")
	#~ print (gi.repository)
#	print (gi.repository)
	#~ c = PyGreeter()
	#~ c.greet()
#	c.greet

def __init__():
    print("foo")
