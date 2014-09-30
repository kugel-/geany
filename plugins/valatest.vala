using Gtk;
using GLib;
using Peas;
using Geany;

public class ValaTest : GLib.Object, Geany.Plugin2
{
	GeanyGI.KeyGroup m_keys = new GeanyGI.KeyGroup("vala", "Vala Test", 1);
	public int methods { get { return Geany.Methods.CLEANUP; } }

	bool on_key_press(GeanyGI.KeyBinding key, uint key_id)
	{
		unowned Gtk.Window w = Geany.data.main_widgets.window as Gtk.Window;
		var d = new Gtk.Dialog.with_buttons("ValaTest", w,
											Gtk.DialogFlags.MODAL, Gtk.Stock.OK, Gtk.ResponseType.OK, null);
		var c = d.get_content_area() as Gtk.Container;
		c.add(new Gtk.Label("Vala Test"));
		d.show_all();
		d.run();
		d.destroy();

		return true;
	}

	public void initialize()
	{
		unowned Document doc = Document.get_current();
		if (doc != null)
		{
			stdout.printf("valatest: %s\n", Document.get_current().editor.sci.get_contents(-1));
			stdout.printf("valatest gi: %s\n", GeanyGI.Document.get_current().editor.sci.get_contents(-1));
		}
		m_keys.add_keybinding(new GeanyGI.KeyBinding("test", "Test", on_key_press), 0);

		Geany.Object o = (Geany.Object) Geany.object;
		o.document_new.connect( (doc) => { stdout.printf("new document\n"); });
		GeanyGI.Document.signals().document_new.connect( (doc) => { stdout.printf("new gi document\n"); });

		stdout.printf("foo %d\n", (int)Geany.data.prefs.load_session);
		stdout.printf("bar %s\n", Geany.data.tool_prefs.browser_cmd);
		stdout.printf("bar %s\n", GeanyGI.Data.instance().tool_prefs.browser_cmd);
		stdout.printf("bar %s\n", GeanyGI.Data.instance().template_prefs.developer);
		stdout.printf("configdir: %s\n", GeanyGI.Data.instance().app.configdir);
	}

	public int version_check(int abi)
	{
		return Plugin.version_check(abi, Plugin.API_VERSION);
	}

	public void cleanup()
	{
		/* have to manually unref before destructor, to break dependency cycle
		 * KeyGroup <- KeyBinding <- this <- KeyGroup */
		m_keys = null;
		stdout.printf("cleanup!\n");
	}

	construct { stdout.printf("ValaTest()\n"); }
	~ValaTest() { stdout.printf("~ValaTest()\n"); }
}


public void peas_register_types(Peas.ObjectModule mod)
{
	mod.register_extension_type(typeof(Geany.Plugin2), typeof(ValaTest));
}
