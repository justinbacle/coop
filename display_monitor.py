import urwid
import time
import random


class heatBar(urwid.ProgressBar):
    def __init__(self, normal='normal', complete='complete', current=0, done=100, satt=None, title=''):
        self.title = title
        super().__init__(normal, complete, current=current, done=done, satt=satt)

    def get_text(self):
        percent = min(100, max(0, int(self.current * 100 / self.done)))
        return f"{self.title} : {percent} °C"


class TuiMonitor():

    items = []
    cells = []

    def __init__(self):
        self.addcells([f"bar n°{n}" for n in range(5)])
        self.mainBody = urwid.Pile(self.cells)

        self.setGlobalLayout()

        # init display
        self.screen = urwid.raw_display.Screen()
        self.screen.set_terminal_properties(16)

        self.palette = [
            ('bg', 'dark blue', 'black'),
            ('header', 'white', 'dark gray'),
            ('input', 'white', 'dark blue'),
            ('logged_input', 'dark magenta', 'black'),
            ('logged_response', 'light magenta', 'black'),
            ('error', 'yellow', 'dark red'),
            ('default', 'light gray', 'black'),
            ('severity0', 'dark blue', 'black'),
            ('severity1', 'dark green', 'black'),
            ('severity2', 'dark cyan', 'black'),
            ('severity3', 'light gray', 'black'),
            ('severity4', 'light red', 'black'),
            ('severity5', 'yellow', 'dark red'),
            ('highlight', 'black', 'yellow'),
            ('normal', 'white', 'black'),
            ('complete', 'white', 'dark red'),
        ]

        self.main_loop = urwid.MainLoop(self.mainFrame, self.palette, unhandled_input=self._handleInput)

    def addcells(self, names=[]):
        self.bars = []
        for name in names:
            bar = heatBar(title=name)
            self.bars.append(bar)
            self.cells.append(urwid.Filler(bar))

    # Handle key presses
    def _handleInput(self, key):
        if key == 'Q' or key == 'q':
            raise urwid.ExitMainLoop()

    def setGlobalLayout(self):
        # ------------ HEADER ------------
        header_text = urwid.Text(('header', u'COOP Heating Control'))
        self.header = urwid.AttrMap(header_text, 'titlebar')
        self.items.append(self.header)

        # ----------- FOOTER -------------
        self.footer = urwid.Text([u'Press (', ('quit button', u'Q'), u') to quit.'])
        self.items.append(self.footer)

        # ----------- MAIN LAYOUT -------------
        self.mainFrame = urwid.Frame(header=self.header, body=self.mainBody, footer=self.footer)

    def _refresh(self, _loop, _data):
        self.main_loop.draw_screen()
        timeText = u''.join(str(time.ctime()))
        self.footer.set_text(timeText)
        for item in self.items:
            try:
                item.refresh()
            except AttributeError:
                pass
        self.main_loop.set_alarm_in(1, self._refresh)

    def _randomFill(self, _loop, _data):
        for bar in self.bars:
            bar.set_completion(random.randint(0, 80))
        self.main_loop.set_alarm_in(1, self._randomFill)

    def run(self):
        self.main_loop.set_alarm_in(1, self._refresh)
        self.main_loop.set_alarm_in(1, self._randomFill)
        self.main_loop.run()


tui = TuiMonitor()
tui.run()
