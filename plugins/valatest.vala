using Gtk;
using GLib;
using Peas;
using Geany;

public class ValaTest : GLib.Object, Geany.Plugin2
{
	public int methods { get { return Geany.Methods.CLEANUP; } }
	public void initialize()
	{
		unowned Gtk.Window w = Geany.data.main_widgets.window as Gtk.Window;
		var d = new Gtk.Dialog.with_buttons("ValaTest", w,
											Gtk.DialogFlags.MODAL, Gtk.Stock.OK, Gtk.ResponseType.OK, null);
		var c = d.get_content_area() as Gtk.Container;
		c.add(new Gtk.Label("Vala Test"));
		d.show_all();
		d.run();
		d.destroy();

		Geany.Object o = (Geany.Object) Geany.object;
		o.document_new.connect( (doc) => { stdout.printf("new document\n"); });
	}

	public int version_check(int abi)
	{
		return Plugin.version_check(abi, Plugin.API_VERSION);
	}

	public void cleanup()
	{
		stdout.printf("cleanup!\n");
	}
}


public void peas_register_types(Peas.ObjectModule mod)
{
	mod.register_extension_type(typeof(Geany.Plugin2), typeof(ValaTest));
}
