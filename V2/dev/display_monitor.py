import urwid
import time
import random
import lorem


'''
Todo :
Make two Piles, one for bars, and a higher one that includes the barsPile and bottom text event viewer
'''


class heatBar(urwid.ProgressBar):
    def __init__(self, normal='normal', complete='complete', current=0, done=100, satt=None, title=''):
        self.title = title
        super().__init__(normal, complete, current=current, done=done, satt=satt)

    def get_text(self):
        percent = min(100, max(0, int(self.current * 100 / self.done)))
        return f"{self.title} : {percent} °C"


class TuiMonitor():

    refreshItems = []
    cells = []

    def __init__(self):

        # Mode Boxes
        self.modeButtons = urwid.SimpleFocusListWalker([])
        for _t in ['a', 'b', 'c', 'd']:
            # b = urwid.CheckBox(_t)  # check box keep focus
            b = urwid.Text(_t.upper())  # need to make custom class
            self.modeButtons.append(b)
        self.modeBoxes = urwid.LineBox(
            urwid.Filler(urwid.Columns(self.modeButtons)), title="Modes", title_align='center')
        self.cells.append(self.modeBoxes)

        # Heat Bars
        self.addcells([f"bar n°{n}" for n in range(5)])

        # Text log
        self.textLog = urwid.Text('')
        self.refreshItems.append(self.textLog)
        self.cells.append(urwid.LineBox(urwid.Filler(self.textLog), title='Log', title_align='center'))

        self.mainBody = urwid.LineBox(urwid.Pile(self.cells))
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
            # self.cells.append(urwid.Filler(bar))
        self.cells.append(urwid.LineBox(urwid.Pile(
            [urwid.Padding(urwid.Filler(bar), left=2, right=2) for bar in self.bars]
            ), title='Temperatures', title_align='center'))

    # Handle key presses
    def _handleInput(self, key):
        if key == 'Q' or key == 'q':
            raise urwid.ExitMainLoop()

    def setGlobalLayout(self):
        # ------------ HEADER ------------
        header_text = urwid.Text(('header', u'COOP Heating Control'))
        self.header = urwid.AttrMap(header_text, 'titlebar')
        self.refreshItems.append(self.header)

        # ----------- FOOTER -------------
        self.footer = urwid.Text([u'Press (', ('quit button', u'Q'), u') to quit.'])
        self.refreshItems.append(self.footer)

        # ----------- MAIN LAYOUT -------------
        self.mainFrame = urwid.Frame(header=self.header, body=self.mainBody, footer=self.footer)

    def _refresh(self, _loop, _data):
        self.main_loop.draw_screen()
        timeText = u''.join(str(time.ctime()))
        self.footer.set_text(timeText)
        for item in self.refreshItems:
            try:
                item.refresh()
            except AttributeError:
                pass
        self.main_loop.set_alarm_in(1, self._refresh)

    def _randomFill(self, _loop, _data):
        for bar in self.bars:
            bar.set_completion(random.randint(0, 80))
        self.main_loop.set_alarm_in(1, self._randomFill)
        self.textLog.set_text(lorem.paragraph())

    def run(self):
        self.main_loop.set_alarm_in(1, self._refresh)
        self.main_loop.set_alarm_in(1, self._randomFill)
        self.main_loop.run()


tui = TuiMonitor()
tui.run()
