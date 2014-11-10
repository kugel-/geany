using Gtk;
using GLib;
using Peas;
using Geany;

public class ValaTest : GLib.Object, Geany.Plugin2
{
	GeanyGI.KeyGroup m_keys = new GeanyGI.KeyGroup("vala", "Vala Test", 1);
	public int methods { get { return Geany.Methods.CLEANUP | Geany.Methods.KEY_GROUP; } }

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

		Geany.LexerStyle *s = Geany.Highlighting.get_style(1, 1);
		stdout.printf("style: #%6X;#%6X;%s;%s\n", s.foreground, s.background,
					  s.bold.to_string(), s.italic.to_string());
		assert(Geany.FiletypeID.PHP == 1);
		Geany.LexerStyle ss = GeanyGI.Highlighting.get_lexer_style(
				GeanyGI.Filetype.get_by_id(Geany.FiletypeID.PHP), Scintilla.SCE_P_COMMENTLINE);
		stdout.printf("style: #%6X;#%6X;%s;%s\n", ss.foreground, ss.background,
					  ss.bold.to_string(), ss.italic.to_string());
	}

	public int version_check(int abi)
	{
		return Plugin.version_check(abi, Plugin.API_VERSION);
	}

	public string key_group()
	{
		return m_keys.label;
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
