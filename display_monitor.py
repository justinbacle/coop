import urwid
import time


class TuiMonitor():

    items = []

    def __init__(self):
        self.quote_text = urwid.Text(u'Press (R) to get your first quote!')
        quote_filler = urwid.Filler(self.quote_text, valign='top', top=1, bottom=1)
        v_padding = urwid.Padding(quote_filler, left=1, right=1)
        self.mainBody = urwid.LineBox(v_padding)

        self.setGLobalLayout()

        # Set up color scheme
        palette = []

        self.main_loop = urwid.MainLoop(self.mainFrame, palette, unhandled_input=self._handleInput)

    # Handle key presses
    def _handleInput(self, key):
        if key == 'Q' or key == 'q':
            raise urwid.ExitMainLoop()

    def setGLobalLayout(self):
        # ------------ HEADER ------------
        header_text = urwid.Text(u'Test Program')
        self.header = urwid.AttrMap(header_text, 'titlebar')
        self.items.append(self.header)

        # ----------- FOOTER -------------
        self.footer = urwid.Text([
            u'Press (', ('quit button', u'Q'), u') to quit.'
        ])
        self.items.append(self.footer)

        # ----------- MAIN LAYOUT -------------
        self.mainFrame = urwid.Frame(header=self.header, body=self.mainBody, footer=self.footer)

    def _refresh(self, _loop, _data):
        self.main_loop.draw_screen()
        timeText = u''.join(str(time.ctime()))
        self.footer.set_text(timeText)
        self.main_loop.set_alarm_in(1, self._refresh)

    def run(self):
        self.main_loop.set_alarm_in(1, self._refresh)
        self.main_loop.run()


tui = TuiMonitor()
tui.run()
