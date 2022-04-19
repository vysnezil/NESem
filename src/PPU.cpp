#include "PPU.h"
#include "Logger.h"
#include "Input.h"

void PPU::loadCartridge(Cartridge* cartridge)
{
	cart = cartridge;
}

Save::PPUstate PPU::getState() {
	Save::PPUstate state = {
		status.reg, mask.reg, control.reg,
		(uint16_t)vram_addr.reg, (uint16_t)tram_addr.reg,
		fine_x,
		dat, *OAM,
		**tblName, **tblPattern, *tblPalette
	};
	return state;
}

void PPU::loadState(Save::PPUstate state) {
	this->reset();
	/*memcpy(OAM, state.OAM, 64 * sizeof(Save::Sprite));
	memcpy(tblName, state.tblName, 2*1024);
	memcpy(tblPattern, state.tblPattern, 2*4096);
	memcpy(tblPalette, state.tblPalette, 32);*/
	status.reg = state.status;
	mask.reg = state.mask;
	control.reg = state.control;
	vram_addr.reg = state.vram_addr;
	tram_addr.reg = state.tram_addr;
	fine_x = state.fine_x;
}

void PPU::reset()
{
	fine_x = 0x00;
	address_latch = 0x00;
	ppu_data_buffer = 0x00;
	scanline = 0;
	cycle = 0;
	bg_next_tile_id = 0x00;
	bg_next_tile_attrib = 0x00;
	bg_next_tile_lsb = 0x00;
	bg_next_tile_msb = 0x00;
	bg_shifter_pattern_lo = 0x0000;
	bg_shifter_pattern_hi = 0x0000;
	bg_shifter_attrib_lo = 0x0000;
	bg_shifter_attrib_hi = 0x0000;
	status.reg = 0x00;
	mask.reg = 0x00;
	control.reg = 0x00;
	vram_addr.reg = 0x0000;
	tram_addr.reg = 0x0000;

	palScreen[0x00] = Color(84, 84, 84);
	palScreen[0x01] = Color(0, 30, 116);
	palScreen[0x02] = Color(8, 16, 144);
	palScreen[0x03] = Color(48, 0, 136);
	palScreen[0x04] = Color(68, 0, 100);
	palScreen[0x05] = Color(92, 0, 48);
	palScreen[0x06] = Color(84, 4, 0);
	palScreen[0x07] = Color(60, 24, 0);
	palScreen[0x08] = Color(32, 42, 0);
	palScreen[0x09] = Color(8, 58, 0);
	palScreen[0x0A] = Color(0, 64, 0);
	palScreen[0x0B] = Color(0, 60, 0);
	palScreen[0x0C] = Color(0, 50, 60);
	palScreen[0x0D] = Color(0, 0, 0);
	palScreen[0x0E] = Color(0, 0, 0);
	palScreen[0x0F] = Color(0, 0, 0);

	palScreen[0x10] = Color(152, 150, 152);
	palScreen[0x11] = Color(8, 76, 196);
	palScreen[0x12] = Color(48, 50, 236);
	palScreen[0x13] = Color(92, 30, 228);
	palScreen[0x14] = Color(136, 20, 176);
	palScreen[0x15] = Color(160, 20, 100);
	palScreen[0x16] = Color(152, 34, 32);
	palScreen[0x17] = Color(120, 60, 0);
	palScreen[0x18] = Color(84, 90, 0);
	palScreen[0x19] = Color(40, 114, 0);
	palScreen[0x1A] = Color(8, 124, 0);
	palScreen[0x1B] = Color(0, 118, 40);
	palScreen[0x1C] = Color(0, 102, 120);
	palScreen[0x1D] = Color(0, 0, 0);
	palScreen[0x1E] = Color(0, 0, 0);
	palScreen[0x1F] = Color(0, 0, 0);

	palScreen[0x20] = Color(236, 238, 236);
	palScreen[0x21] = Color(76, 154, 236);
	palScreen[0x22] = Color(120, 124, 236);
	palScreen[0x23] = Color(176, 98, 236);
	palScreen[0x24] = Color(228, 84, 236);
	palScreen[0x25] = Color(236, 88, 180);
	palScreen[0x26] = Color(236, 106, 100);
	palScreen[0x27] = Color(212, 136, 32);
	palScreen[0x28] = Color(160, 170, 0);
	palScreen[0x29] = Color(116, 196, 0);
	palScreen[0x2A] = Color(76, 208, 32);
	palScreen[0x2B] = Color(56, 204, 108);
	palScreen[0x2C] = Color(56, 180, 204);
	palScreen[0x2D] = Color(60, 60, 60);
	palScreen[0x2E] = Color(0, 0, 0);
	palScreen[0x2F] = Color(0, 0, 0);

	palScreen[0x30] = Color(236, 238, 236);
	palScreen[0x31] = Color(168, 204, 236);
	palScreen[0x32] = Color(188, 188, 236);
	palScreen[0x33] = Color(212, 178, 236);
	palScreen[0x34] = Color(236, 174, 236);
	palScreen[0x35] = Color(236, 174, 212);
	palScreen[0x36] = Color(236, 180, 176);
	palScreen[0x37] = Color(228, 196, 144);
	palScreen[0x38] = Color(204, 210, 120);
	palScreen[0x39] = Color(180, 222, 120);
	palScreen[0x3A] = Color(168, 226, 144);
	palScreen[0x3B] = Color(152, 226, 180);
	palScreen[0x3C] = Color(160, 214, 228);
	palScreen[0x3D] = Color(160, 162, 160);
	palScreen[0x3E] = Color(0, 0, 0);
	palScreen[0x3F] = Color(0, 0, 0);

}

PPU::PPU()
{
}


uint8_t PPU::cpuRead(uint16_t addr)
{
	uint8_t data = 0x00;

	switch (addr)
	{
	case 0x0002:
		data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
		status.vertical_blank = 0;
		address_latch = 0;
		break;
	case 0x0004:
		data = ((uint8_t*)OAM)[oam_addr];
		break;
	case 0x0007:
		data = ppu_data_buffer;
		ppu_data_buffer = ppuRead(vram_addr.reg);
		if (vram_addr.reg >= 0x3F00) data = ppu_data_buffer;
		vram_addr.reg += (control.increment_mode ? 32 : 1);
		break;
	}

	return data;
}

void PPU::cpuWrite(uint16_t addr, uint8_t data)
{
	switch (addr)
	{
	case 0x0000: // Control
		control.reg = data;
		tram_addr.nametable_x = control.nametable_x;
		tram_addr.nametable_y = control.nametable_y;
		break;
	case 0x0001: // Mask
		mask.reg = data;
		break;
	case 0x0003:
		oam_addr = data;
		break;
	case 0x0004:
		((uint8_t*)OAM)[oam_addr] = data;
		break;
	case 0x0005: // Scroll
		if (address_latch == 0)
		{
			fine_x = data & 0x07;
			tram_addr.coarse_x = data >> 3;
			address_latch = 1;
		}
		else
		{
			tram_addr.fine_y = data & 0x07;
			tram_addr.coarse_y = data >> 3;
			address_latch = 0;
		}
		break;
	case 0x0006:
		if (address_latch == 0)
		{
			tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
			address_latch = 1;
		}
		else
		{
			tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
			vram_addr = tram_addr;
			address_latch = 0;
		}
		break;
	case 0x0007:
		ppuWrite(vram_addr.reg, data);
		vram_addr.reg += (control.increment_mode ? 32 : 1);
		break;
	}
}

uint8_t PPU::ppuRead(uint16_t addr)
{
	uint8_t data = 0x00;
	addr &= 0x3FFF;
	

	if (cart->ppuRead(addr, data))
	{

	}

	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF)
	{
		addr &= 0x0FFF;

		if (cart->mirror == Cartridge::MIRROR::VERTICAL)
		{
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = tblName[1][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = tblName[1][addr & 0x03FF];
		}
		else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL)
		{
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = tblName[1][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = tblName[1][addr & 0x03FF];
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
	{
		addr &= 0x001F;
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		data = tblPalette[addr] & (mask.grayscale ? 0x30 : 0x3F);
	}

	return data;
}

void PPU::ppuWrite(uint16_t addr, uint8_t data)
{
	addr &= 0x3FFF;

	if (cart->ppuWrite(addr, data))
	{
		
	}

	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF)
	{
		addr &= 0x0FFF;
		if (cart->mirror == Cartridge::MIRROR::VERTICAL)
		{
			// Vertical
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = data;
		}
		else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL)
		{
			// Horizontal
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = data;
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
	{
		addr &= 0x001F;
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		tblPalette[addr] = data;
	}

}

void PPU::clock()
{
	//sprScreen.SetPixel(cycle - 1, scanline, palScreen[(rand() % 2) ? 0x3F : 0x30]);
	

	if (scanline >= -1 && scanline < 240)
	{
		if (cycle == 0)
		{
			cycle = 1;
		}

		if (scanline == -1 && cycle == 1)
		{
			status.vertical_blank = 0;

			status.sprite_overflow = 0;
			status.sprite_zero_hit = 0;


			for (int i = 0; i < 8; i++)
			{
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}
		}


		if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
		{
			if (mask.render_background)
			{
				bg_shifter_pattern_lo <<= 1;
				bg_shifter_pattern_hi <<= 1;

				bg_shifter_attrib_lo <<= 1;
				bg_shifter_attrib_hi <<= 1;
			}

			if (mask.render_sprites && cycle >= 1 && cycle < 258)
			{
				for (int i = 0; i < spriteCount; i++)
				{
					if (spriteScanline[i].x > 0)
					{
						spriteScanline[i].x--;
					}
					else
					{
						sprite_shifter_pattern_lo[i] <<= 1;
						sprite_shifter_pattern_hi[i] <<= 1;
					}
				}
			}

			switch ((cycle - 1) % 8)
			{
			case 0:
				bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
				bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;
				bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
				bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);

				bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));

				break;
			case 2:
			
				bg_next_tile_attrib = ppuRead(0x23C0 | (vram_addr.nametable_y << 11)
					| (vram_addr.nametable_x << 10)
					| ((vram_addr.coarse_y >> 2) << 3)
					| (vram_addr.coarse_x >> 2));
		
				if (vram_addr.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
				if (vram_addr.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
				bg_next_tile_attrib &= 0x03;
				break;


			case 4:
				bg_next_tile_lsb = ppuRead((control.pattern_background << 12)
					+ ((uint16_t)bg_next_tile_id << 4)
					+ (vram_addr.fine_y) + 0);

				break;
			case 6:
				bg_next_tile_msb = ppuRead((control.pattern_background << 12)
					+ ((uint16_t)bg_next_tile_id << 4)
					+ (vram_addr.fine_y) + 8);
				break;
			case 7:
				if (mask.render_background || mask.render_sprites)
				{
					if (vram_addr.coarse_x == 31)
					{
						vram_addr.coarse_x = 0;
						vram_addr.nametable_x = ~vram_addr.nametable_x;
					}
					else
					{
						vram_addr.coarse_x++;
					}
				}
				break;
			}
		}

		if (cycle == 256)
		{
			if (mask.render_background || mask.render_sprites)
			{
				if (vram_addr.fine_y < 7)
				{
					vram_addr.fine_y++;
				}
				else
				{
					vram_addr.fine_y = 0;
					if (vram_addr.coarse_y == 29)
					{
						vram_addr.coarse_y = 0;
						vram_addr.nametable_y = ~vram_addr.nametable_y;
					}
					else if (vram_addr.coarse_y == 31)
					{
						vram_addr.coarse_y = 0;
					}
					else
					{
						vram_addr.coarse_y++;
					}
				}
			}
		}

		if (cycle == 257)
		{
			bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
			bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;
			bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
			bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);


			if (mask.render_background || mask.render_sprites)
			{
				vram_addr.nametable_x = tram_addr.nametable_x;
				vram_addr.coarse_x = tram_addr.coarse_x;
			}
		}

		if (cycle == 338 || cycle == 340)
		{
			bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
		}

		if (scanline == -1 && cycle >= 280 && cycle < 305)
		{
			if (mask.render_background || mask.render_sprites)
			{
				vram_addr.fine_y = tram_addr.fine_y;
				vram_addr.nametable_y = tram_addr.nametable_y;
				vram_addr.coarse_y = tram_addr.coarse_y;
			}
		}


	}

	processForeground();

	if (scanline >= 241 && scanline < 261)
	{
		if (scanline == 241 && cycle == 1)
		{
			status.vertical_blank = 1;
			if (control.enable_nmi) nmi = true;
		}
	}

	uint8_t bg_pixel = 0x00;
	uint8_t bg_palette = 0x00;

	if (mask.render_background)
	{
		uint16_t bit_mux = 0x8000 >> fine_x;
		uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
		uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;
		bg_pixel = (p1_pixel << 1) | p0_pixel;

		uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
		uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
		bg_palette = (bg_pal1 << 1) | bg_pal0;

	}

	renderForeground();

	uint8_t pixel = 0x00;
	uint8_t palette = 0x00;

	if (bg_pixel == 0 && fg_pixel == 0)
	{
		pixel = 0x00;
		palette = 0x00;
	}
	else if (bg_pixel == 0 && fg_pixel > 0)
	{
		pixel = fg_pixel;
		palette = fg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel == 0)
	{
		pixel = bg_pixel;
		palette = bg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel > 0)
	{
		if (fg_priority)
		{
			pixel = fg_pixel;
			palette = fg_palette;
		}
		else
		{
			pixel = bg_pixel;
			palette = bg_palette;
		}

		if (spriteZeroHitPossible && spriteZeroBeingRendered)
		{
			if (mask.render_background & mask.render_sprites)
			{
				if (~(mask.render_background_left | mask.render_sprites_left))
				{
					if (cycle >= 9 && cycle < 258)
					{
						status.sprite_zero_hit = 1;
					}
				}
				else
				{
					if (cycle >= 1 && cycle < 258)
					{
						status.sprite_zero_hit = 1;
					}
				}
			}
		}
	}

	//Logger::logHex(palScreen[ppuRead(0x3F00 + (bg_palette << 2) + bg_pixel) & 0x3F]);

	//if (scanline == 0xC0) Input::singleStep = true;

	Color color = palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
	if (scanline >= 0 && scanline < 240 && cycle <= 256) {
		
		int index = (scanline * 256 + cycle - 1) * 3;
		dat[index + 0] = color.r;
		dat[index + 1] = color.g;
		dat[index + 2] = color.b;

		/*if (true) {
			int index = (scanline * 256 + cycle - 1) * 3;
			dat[index + 0] = 255;
			dat[index + 1] = 0;
			dat[index + 2] = 0;
		}*/

		//prev = color;
		/*
		for (uint8_t i = 0; i < spriteCount; i++)
		{
			if (cycle == OAM[i].x) {
				dat[index + 0] = 255;
				dat[index + 1] = 0;
				dat[index + 2] = 0;
			}
		if (scanline == OAM[i].y) {
			dat[index + 0] = 255;
			dat[index + 1] = 0;
			dat[index + 2] = 0;
		}
		}*/
	}

	

	cycle++;
	if (cycle >= 341)
	{
		cycle = 0;
		scanline++;
		if (scanline >= 261)
		{
			scanline = -1;
			//Logger::log(frames);
			finished = true;
		}
	}
}

void PPU::processForeground() {
	if (cycle == 257 && scanline >= 0)
	{
		std::memset(spriteScanline, 0xFF, 8 * sizeof(Save::Sprite));
		spriteCount = 0;
		for (uint8_t i = 0; i < 8; i++)
		{
			sprite_shifter_pattern_lo[i] = 0;
			sprite_shifter_pattern_hi[i] = 0;
		}

		uint8_t OAMEntry = 0;
		spriteZeroHitPossible = false;
		while (OAMEntry < 64 && spriteCount < 9)
		{
			int16_t diff = ((int16_t)scanline - (int16_t)OAM[OAMEntry].y);
			if (diff >= 0 && diff < (control.sprite_size ? 16 : 8))
			{
				if (OAMEntry == 0) spriteZeroHitPossible = true;
				if (spriteCount < 8)
				{
					memcpy(&spriteScanline[spriteCount], &OAM[OAMEntry], sizeof(Save::Sprite));
					spriteCount++;
				}
			}

			OAMEntry++;
		}
		status.sprite_overflow = (spriteCount > 8);
	}

	if (cycle == 340)
	{
		for (uint8_t i = 0; i < spriteCount; i++)
		{
			uint8_t sprite_pattern_bits_lo, sprite_pattern_bits_hi;
			uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;
			if (!control.sprite_size)
			{
				if (!(spriteScanline[i].attribute & 0x80))
				{
					sprite_pattern_addr_lo =
						(control.pattern_sprite << 12)
						| (spriteScanline[i].id << 4)
						| (scanline - spriteScanline[i].y);

				}
				else
				{
					sprite_pattern_addr_lo =
						(control.pattern_sprite << 12)
						| (spriteScanline[i].id << 4)
						| (7 - (scanline - spriteScanline[i].y));
				}

			}
			else
			{
				if (!(spriteScanline[i].attribute & 0x80))
				{
					if (scanline - spriteScanline[i].y < 8)
					{
						sprite_pattern_addr_lo =
							((spriteScanline[i].id & 0x01) << 12)
							| ((spriteScanline[i].id & 0xFE) << 4)
							| ((scanline - spriteScanline[i].y) & 0x07);
					}
					else
					{
						sprite_pattern_addr_lo =
							((spriteScanline[i].id & 0x01) << 12)
							| (((spriteScanline[i].id & 0xFE) + 1) << 4)
							| ((scanline - spriteScanline[i].y) & 0x07);
					}
				}
				else
				{
					if (scanline - spriteScanline[i].y < 8)
					{
						sprite_pattern_addr_lo =
							((spriteScanline[i].id & 0x01) << 12)
							| (((spriteScanline[i].id & 0xFE) + 1) << 4)
							| (7 - (scanline - spriteScanline[i].y) & 0x07);
					}
					else
					{
						sprite_pattern_addr_lo =
							((spriteScanline[i].id & 0x01) << 12)
							| ((spriteScanline[i].id & 0xFE) << 4)
							| (7 - (scanline - spriteScanline[i].y) & 0x07);
					}
				}
			}

			sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;

			sprite_pattern_bits_lo = ppuRead(sprite_pattern_addr_lo);
			sprite_pattern_bits_hi = ppuRead(sprite_pattern_addr_hi);

			if (spriteScanline[i].attribute & 0x40)
			{
				auto flipbyte = [](uint8_t b)
				{
					b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
					b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
					b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
					return b;
				};

				sprite_pattern_bits_lo = flipbyte(sprite_pattern_bits_lo);
				sprite_pattern_bits_hi = flipbyte(sprite_pattern_bits_hi);
			}

			sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
			sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;
		}
	}
}

void PPU::renderForeground() {
	if (mask.render_sprites)
	{
		spriteZeroBeingRendered = false;
		for (uint8_t i = 0; i < spriteCount; i++)
		{
			if (spriteScanline[i].x == 0)
			{
				uint8_t fg_pixel_lo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
				uint8_t fg_pixel_hi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;
				fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;

				fg_palette = (spriteScanline[i].attribute & 0x03) + 0x04;
				fg_priority = (spriteScanline[i].attribute & 0x20) == 0;
			}
			if (fg_pixel != 0)
			{
				if (i == 0) spriteZeroBeingRendered = true;
				break;
			}
		}
	}
}