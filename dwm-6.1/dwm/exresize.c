#define IS_SET(q, w) ((q & w) != 0)
#define IS_FORCED(q, w) IS_SET((q << 1), w)

enum { EX_NW, EX_N, EX_NE, EX_W, EX_C, EX_E, EX_SW, EX_S, EX_SE };

void exresize(const Arg *arg);
void explace(const Arg *arg);

void
exresize(const Arg *arg) {
        Client *c;
        int x, y, nx, ny, nw, nh;

        /* Change state of maximize before exresize. */
        /* if(!selmon->sel->ismax) { */
        /*         if(!selmon->lt[selmon->sellt]->arrange || selmon->sel->isfloating) */
        /*                 selmon->sel->wasfloating = True; */
        /*         else { */
        /*                 togglefloating(NULL); */
        /*                 selmon->sel->wasfloating = False; */
        /*         } */
        /*         selmon->sel->ismax = True; */
        /* } */

        c = selmon->sel;

        if (!c || !arg) return;
        if (selmon->lt[selmon->sellt]->arrange && !c->isfloating)
                togglefloating(NULL);

        x = ((int *)arg->v)[0];
        y = ((int *)arg->v)[1];

        nw = MIN(selmon->ww - c->bw*2, c->w + x);
        nh = MIN(selmon->wh - c->bw*2, c->h + y);
        nx = c->x - x/2;
        ny = c->y - y/2;

        if (!((abs(c->x + c->w/2 - (selmon->wx + selmon->ww/2)) < snap))) {
                if ((nw == selmon->ww) ||
                    (nx < selmon->wx) ||
                    (abs(selmon->wx - c->x) < snap))
                        nx = selmon->wx;
                else if ((nx+nw > (selmon->wx + selmon->ww)) ||
                         (abs((selmon->wx + selmon->ww) - (c->x + c->w)) < snap))
                        nx = (selmon->wx + selmon->ww) - nw - c->bw*2;
        } else
                nx = selmon->wx + selmon->ww/2 - nw/2;

        if (!((abs(c->y + c->h/2 - (selmon->wy + selmon->wh/2)) < snap))) {

                if ((nh == selmon->wh) ||
                    (ny < selmon->wy) ||
                    (abs(selmon->wy - c->y) < snap))
                        ny = selmon->wy;
                else if ((ny+nh > (selmon->wy + selmon->wh)) ||
                         (abs((selmon->wy + selmon->wh) - (c->y + c->h)) < snap))
                        ny = (selmon->wy + selmon->wh) - nh - c->bw*2;
        } else
                ny = selmon->wy + selmon->wh/2 - nh/2;

        resizeclient(c, nx, ny, MAX(nw, 32), MAX(nh, 32));
        XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w/2, c->h/2);
}

void
explace(const Arg *arg) {
        Client *c;
        int nx, ny;

        /* Change state of maximize before explace. */
        /* if(!selmon->sel->ismax) { */
        /*         if(!selmon->lt[selmon->sellt]->arrange || selmon->sel->isfloating) */
        /*                 selmon->sel->wasfloating = True; */
        /*         else { */
        /*                 togglefloating(NULL); */
        /*                 selmon->sel->wasfloating = False; */
        /*         } */
        /*         selmon->sel->ismax = True; */
        /* } */

        c = selmon->sel;
        if (!c || (arg->ui >= 9)) return;
        if (selmon->lt[selmon->sellt]->arrange && !c->isfloating)
                togglefloating(NULL);

        nx = (arg->ui % 3) - 1;
        ny = (arg->ui / 3) - 1;

        if (nx < 0) nx = selmon->wx;
        else if (nx > 0) nx = selmon->wx + selmon->ww - c->w - c->bw*2;
        else nx = selmon->wx + selmon->ww/2 - c->w/2;

        if (ny < 0) ny = selmon->wy;
        else if (ny > 0) ny = selmon->wy + selmon->wh - c->h - c->bw*2;
        else ny = selmon->wy + selmon->wh/2 - c->h/2;

        resize(c, nx, ny, c->w, c->h, True);
        XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w/2, c->h/2);
}
