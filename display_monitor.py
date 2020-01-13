import urwid
import time


class TuiMonitor():

    # Handle key presses
    def _handleInput(key):
        if key == 'Q' or key == 'q':
            raise urwid.ExitMainLoop()

    def __init__(self):
        header_text = urwid.Text(u'Test Program')
        header = urwid.AttrMap(header_text, 'titlebar')

        self.bottomText = urwid.Text([
            u'Press (', ('quit button', u'Q'), u') to quit.'
        ])

        self.quote_text = urwid.Text(u'Press (R) to get your first quote!')
        quote_filler = urwid.Filler(self.quote_text, valign='top', top=1, bottom=1)
        v_padding = urwid.Padding(quote_filler, left=1, right=1)
        self.quote_box = urwid.LineBox(v_padding)

        # Set up color scheme
        palette = []

        layout = urwid.Frame(header=header, body=self.quote_box, footer=self.bottomText)
        self.main_loop = urwid.MainLoop(layout, palette, unhandled_input=self._handleInput)

    def _refresh(self, _loop, _data):
        self.main_loop.draw_screen()
        timeText = u''.join(str(time.ctime()))
        self.quote_box.base_widget.set_text(timeText)
        self.main_loop.set_alarm_in(1, self._refresh)

    def run(self):
        self.main_loop.set_alarm_in(1, self._refresh)
        self.main_loop.run()


tui = TuiMonitor()
tui.run()
