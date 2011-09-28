/*
 * kernel/graphic/software/sw_blit.c
 *
 * Copyright (c) 2007-2011  jianjun jiang <jerryjianjun@gmail.com>
 * official site: http://xboot.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <graphic/software.h>

static void blit_replace_generic(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	struct color_t col;
	u32_t dc, sc;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	for(j = 0; j < dh; j++)
	{
		for(i = 0; i < dw; i++)
		{
			sc = surface_sw_get_pixel(src, sx + i, sy + j);
			unmap_pixel_color(&src->info, sc, &col);
			dc = map_pixel_color(&dst->info, &col);
			surface_sw_set_pixel(dst, dx + i, dy + j, dc);
		}
	}
}

static void blit_replace_directN(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dpitch, spitch;
	s32_t j, len;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dpitch = dst->pitch;
	spitch = src->pitch;
	len = dw * dst->info.bytes_per_pixel;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for(j = 0; j < dh; j++)
	{
		memmove(dp, sp, len);

		dp += dpitch;
		sp += spitch;
	}
}

static void blit_replace_ABGR_8888_ARGB_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			b = *sp++;
			g = *sp++;
			r = *sp++;
			a = *sp++;

			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
			*dp++ = a;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_ABGR_8888_BGRA_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			a = *sp++;
			r = *sp++;
			g = *sp++;
			b = *sp++;

			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
			*dp++ = a;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_ABGR_8888_RGBA_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			a = *sp++;
			b = *sp++;
			g = *sp++;
			r = *sp++;

			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
			*dp++ = a;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_ABGR_8888_BGR_888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			r = *sp++;
			g = *sp++;
			b = *sp++;

			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
			*dp++ = 0xff;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_ABGR_8888_RGB_888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			b = *sp++;
			g = *sp++;
			r = *sp++;

			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
			*dp++ = 0xff;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_ARGB_8888_ABGR_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			r = *sp++;
			g = *sp++;
			b = *sp++;
			a = *sp++;

			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
			*dp++ = a;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_ARGB_8888_BGRA_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			a = *sp++;
			r = *sp++;
			g = *sp++;
			b = *sp++;

			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
			*dp++ = a;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_ARGB_8888_RGBA_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			a = *sp++;
			b = *sp++;
			g = *sp++;
			r = *sp++;

			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
			*dp++ = a;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_ARGB_8888_BGR_888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			r = *sp++;
			g = *sp++;
			b = *sp++;

			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
			*dp++ = 0xff;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_ARGB_8888_RGB_888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			b = *sp++;
			g = *sp++;
			r = *sp++;

			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
			*dp++ = 0xff;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_BGRA_8888_ABGR_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			r = *sp++;
			g = *sp++;
			b = *sp++;
			a = *sp++;

			*dp++ = a;
			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_BGRA_8888_ARGB_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			b = *sp++;
			g = *sp++;
			r = *sp++;
			a = *sp++;

			*dp++ = a;
			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_BGRA_8888_RGBA_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			a = *sp++;
			b = *sp++;
			g = *sp++;
			r = *sp++;

			*dp++ = a;
			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_BGRA_8888_BGR_888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			r = *sp++;
			g = *sp++;
			b = *sp++;

			*dp++ = 0xff;
			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_BGRA_8888_RGB_888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			b = *sp++;
			g = *sp++;
			r = *sp++;

			*dp++ = 0xff;
			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_RGBA_8888_ABGR_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			r = *sp++;
			g = *sp++;
			b = *sp++;
			a = *sp++;

			*dp++ = a;
			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_RGBA_8888_ARGB_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			b = *sp++;
			g = *sp++;
			r = *sp++;
			a = *sp++;

			*dp++ = a;
			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_RGBA_8888_BGRA_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			a = *sp++;
			r = *sp++;
			g = *sp++;
			b = *sp++;

			*dp++ = a;
			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_RGBA_8888_BGR_888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			r = *sp++;
			g = *sp++;
			b = *sp++;

			*dp++ = 0xff;
			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_RGBA_8888_RGB_888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			b = *sp++;
			g = *sp++;
			r = *sp++;

			*dp++ = 0xff;
			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_BGR_888_ABGR_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			r = *sp++;
			g = *sp++;
			b = *sp++;
			a = *sp++;

			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_BGR_888_ARGB_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			b = *sp++;
			g = *sp++;
			r = *sp++;
			a = *sp++;

			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_BGR_888_BGRA_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			a = *sp++;
			r = *sp++;
			g = *sp++;
			b = *sp++;

			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_BGR_888_RGBA_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			a = *sp++;
			b = *sp++;
			g = *sp++;
			r = *sp++;

			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
		}

		dp += dskip;
		sp += sskip;
	}
}


static void blit_replace_BGR_888_RGB_888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			b = *sp++;
			g = *sp++;
			r = *sp++;

			*dp++ = r;
			*dp++ = g;
			*dp++ = b;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_RGB_888_ABGR_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			r = *sp++;
			g = *sp++;
			b = *sp++;
			a = *sp++;

			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_RGB_888_ARGB_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			b = *sp++;
			g = *sp++;
			r = *sp++;
			a = *sp++;

			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_RGB_888_BGRA_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			a = *sp++;
			r = *sp++;
			g = *sp++;
			b = *sp++;

			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_RGB_888_RGBA_8888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b, a;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			a = *sp++;
			b = *sp++;
			g = *sp++;
			r = *sp++;

			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace_RGB_888_BGR_888(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	u8_t * dp, * sp;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	u32_t dskip, sskip;
	u8_t r, g, b;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	dskip = dst->pitch - dst->info.bytes_per_pixel * dw;
	sskip = src->pitch - src->info.bytes_per_pixel * dw;

	sp = surface_sw_get_pointer(src, sx, sy);
	dp = surface_sw_get_pointer(dst, dx, dy);

	for (j = 0; j < dh; j++)
	{
		for (i = 0; i < dw; i++)
		{
			r = *sp++;
			g = *sp++;
			b = *sp++;

			*dp++ = b;
			*dp++ = g;
			*dp++ = r;
		}

		dp += dskip;
		sp += sskip;
	}
}

static void blit_replace(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	switch (dst->info.fmt)
	{
	case PIXEL_FORMAT_ABGR_8888:
	{
		switch (src->info.fmt)
		{
		case PIXEL_FORMAT_ABGR_8888:
			blit_replace_directN(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_ARGB_8888:
			blit_replace_ABGR_8888_ARGB_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_BGRA_8888:
			blit_replace_ABGR_8888_BGRA_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_RGBA_8888:
			blit_replace_ABGR_8888_RGBA_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_BGR_888:
			blit_replace_ABGR_8888_BGR_888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_RGB_888:
			blit_replace_ABGR_8888_RGB_888(dst, dst_rect, src, src_rect);
			return;

		default:
			break;
		}
		break;
	}

	case PIXEL_FORMAT_ARGB_8888:
	{
		switch (src->info.fmt)
		{
		case PIXEL_FORMAT_ABGR_8888:
			blit_replace_ARGB_8888_ABGR_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_ARGB_8888:
			blit_replace_directN(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_BGRA_8888:
			blit_replace_ARGB_8888_BGRA_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_RGBA_8888:
			blit_replace_ARGB_8888_RGBA_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_BGR_888:
			blit_replace_ARGB_8888_BGR_888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_RGB_888:
			blit_replace_ARGB_8888_RGB_888(dst, dst_rect, src, src_rect);
			return;

		default:
			break;
		}
		break;
	}

	case PIXEL_FORMAT_BGRA_8888:
	{
		switch (src->info.fmt)
		{
		case PIXEL_FORMAT_ABGR_8888:
			blit_replace_BGRA_8888_ABGR_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_ARGB_8888:
			blit_replace_BGRA_8888_ARGB_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_BGRA_8888:
			blit_replace_directN(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_RGBA_8888:
			blit_replace_BGRA_8888_RGBA_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_BGR_888:
			blit_replace_BGRA_8888_BGR_888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_RGB_888:
			blit_replace_BGRA_8888_RGB_888(dst, dst_rect, src, src_rect);
			return;

		default:
			break;
		}
		break;
	}

	case PIXEL_FORMAT_RGBA_8888:
	{
		switch (src->info.fmt)
		{
		case PIXEL_FORMAT_ABGR_8888:
			blit_replace_RGBA_8888_ABGR_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_ARGB_8888:
			blit_replace_RGBA_8888_ARGB_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_BGRA_8888:
			blit_replace_RGBA_8888_BGRA_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_RGBA_8888:
			blit_replace_directN(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_BGR_888:
			blit_replace_RGBA_8888_BGR_888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_RGB_888:
			blit_replace_RGBA_8888_RGB_888(dst, dst_rect, src, src_rect);
			return;

		default:
			break;
		}
		break;
	}

	case PIXEL_FORMAT_BGR_888:
	{
		switch (src->info.fmt)
		{
		case PIXEL_FORMAT_ABGR_8888:
			blit_replace_BGR_888_ABGR_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_ARGB_8888:
			blit_replace_BGR_888_ARGB_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_BGRA_8888:
			blit_replace_BGR_888_BGRA_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_RGBA_8888:
			blit_replace_BGR_888_RGBA_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_BGR_888:
			blit_replace_directN(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_RGB_888:
			blit_replace_BGR_888_RGB_888(dst, dst_rect, src, src_rect);
			return;

		default:
			break;
		}
		break;
	}

	case PIXEL_FORMAT_RGB_888:
	{
		switch (src->info.fmt)
		{
		case PIXEL_FORMAT_ABGR_8888:
			blit_replace_RGB_888_ABGR_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_ARGB_8888:
			blit_replace_RGB_888_ARGB_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_BGRA_8888:
			blit_replace_RGB_888_BGRA_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_RGBA_8888:
			blit_replace_RGB_888_RGBA_8888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_BGR_888:
			blit_replace_RGB_888_BGR_888(dst, dst_rect, src, src_rect);
			return;

		case PIXEL_FORMAT_RGB_888:
			blit_replace_directN(dst, dst_rect, src, src_rect);
			return;

		default:
			break;
		}
		break;
	}

	case PIXEL_FORMAT_BGR_565:
	{
		switch (src->info.fmt)
		{
		case PIXEL_FORMAT_BGR_565:
			blit_replace_directN(dst, dst_rect, src, src_rect);
			return;

		default:
			break;
		}
		break;
	}

	case PIXEL_FORMAT_RGB_565:
	{
		switch (src->info.fmt)
		{
		case PIXEL_FORMAT_RGB_565:
			blit_replace_directN(dst, dst_rect, src, src_rect);
			return;

		default:
			break;
		}
		break;
	}

	case PIXEL_FORMAT_BGR_332:
	{
		switch (src->info.fmt)
		{
		case PIXEL_FORMAT_BGR_332:
			blit_replace_directN(dst, dst_rect, src, src_rect);
			return;

		default:
			break;
		}
		break;
	}

	case PIXEL_FORMAT_RGB_332:
	{
		switch (src->info.fmt)
		{
		case PIXEL_FORMAT_RGB_332:
			blit_replace_directN(dst, dst_rect, src, src_rect);
			return;

		default:
			break;
		}
		break;
	}

	default:
		break;
	}

	blit_replace_generic(dst, dst_rect, src, src_rect);
}

static void blit_alpha_generic(struct surface_t * dst, struct rect_t * dst_rect, struct surface_t * src, struct rect_t * src_rect)
{
	struct color_t dcol, scol;
	struct color_t col;
	u8_t alpha;
	u32_t dc, sc;
	s32_t dx, dy, dw, dh;
	s32_t sx, sy;
	s32_t i, j;

	dx = dst_rect->x;
	dy = dst_rect->y;
	dw = dst_rect->w;
	dh = dst_rect->h;

	sx = src_rect->x;
	sy = src_rect->y;

	for(j = 0; j < dh; j++)
	{
		for(i = 0; i < dw; i++)
		{
			sc = surface_sw_get_pixel(src, sx + i, sy + j);
			unmap_pixel_color(&src->info, sc, &scol);

			alpha = scol.a;

			if(alpha == 0)
				continue;

			if(alpha == 0xff)
			{
				dc = map_pixel_color(&dst->info, &scol);
				surface_sw_set_pixel(dst, dx + i, dy + j, dc);
				continue;
			}

			dc = surface_sw_get_pixel(dst, dx + i, dy + j);
			unmap_pixel_color(&dst->info, dc, &dcol);

			col.r = (((scol.r * alpha) + (dcol.r * (255 - alpha))) / 255);
			col.g = (((scol.g * alpha) + (dcol.g * (255 - alpha))) / 255);
			col.b = (((scol.b * alpha) + (dcol.b * (255 - alpha))) / 255);
			col.a = alpha;

			dc = map_pixel_color(&dst->info, &dcol);
			surface_sw_set_pixel(dst, dx + i, dy + j, dc);
		}
	}
}

bool_t software_blit(struct surface_t * dst, struct rect_t * dst_rect,
		struct surface_t * src, struct rect_t * src_rect, enum blit_mode mode)
{
	struct rect_t dst_full;
	s32_t x, y, w, h;

	if(!dst || !src)
		return FALSE;

	if(!dst_rect)
	{
		dst_full.x = 0;
		dst_full.y = 0;
		dst_full.w = dst->w;
		dst_full.h = dst->h;

		dst_rect = &dst_full;
	}

    /* clip the source rectangle to the source surface */
	if (src_rect)
	{
		s32_t maxw, maxh;

		x = src_rect->x;
		w = src_rect->w;
		if (x < 0)
		{
			w += x;
			dst_rect->x -= x;
			x = 0;
		}
		maxw = src->w - x;
		if (maxw < w)
			w = maxw;

		y = src_rect->y;
		h = src_rect->h;
		if (y < 0)
		{
			h += y;
			dst_rect->y -= y;
			y = 0;
		}
		maxh = src->h - y;
		if (maxh < h)
			h = maxh;
	}
	else
	{
		x = y = 0;
		w = src->w;
		h = src->h;
	}

    /* clip the destination rectangle against the clip rectangle */
    {
        struct rect_t * clip = &dst->clip;
        s32_t dx, dy;

        dx = clip->x - dst_rect->x;
        if (dx > 0) {
            w -= dx;
            dst_rect->x += dx;
            x += dx;
        }
        dx = dst_rect->x + w - clip->x - clip->w;
        if (dx > 0)
            w -= dx;

        dy = clip->y - dst_rect->y;
        if (dy > 0) {
            h -= dy;
            dst_rect->y += dy;
            y += dy;
        }
        dy = dst_rect->y + h - clip->y - clip->h;
        if (dy > 0)
            h -= dy;
    }

	if (w > 0 && h > 0)
	{
		struct rect_t sr;
		sr.x = x;
		sr.y = y;
		sr.w = dst_rect->w = w;
		sr.h = dst_rect->h = h;

		blit_replace(dst, dst_rect, src, &sr);
	}

	dst_rect->w = dst_rect->h = 0;
	return TRUE;
}
