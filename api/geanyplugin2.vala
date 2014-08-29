using Gtk;

namespace Geany
{
	public enum Methods
	{
		CONFIGURE        =   1,
		CONFIGURE_SINGLE =   2,
		HELP             =   4,
		CLEANUP          =   8,
	}

	private void w(string msg)
	{
		warning("Buggy plugin does not actually implement " + msg + "()!");
	}

	[CCode (cheader_filename="geanyplugin2.h")]
	public interface Plugin2 : GLib.Object
	{
		public abstract int         methods          { get; }
		public abstract int         version_check    (int abi_ver);
		public abstract void        initialize       ();
		public virtual  Gtk.Widget  configure        (Gtk.Dialog dialog) { w("configure"); return new Gtk.Widget(); }
		public virtual  void        configure_single (Gtk.Widget parent) { w("configure_single"); }
		public virtual  void        help             () { w("help"); }
		public virtual  void        cleanup          () { w("cleanup"); }
	}
}
