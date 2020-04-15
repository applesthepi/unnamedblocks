	sfRenderWindow* window;
	sfVideoMode mode = { 1280, 720, 32 };
	window = sfRenderWindow_create(mode, "---TEST--- Unnamed Blocks Runtime", sfClose, NULL);
	sfRenderWindow_setVerticalSyncEnabled(window, sfFalse);
	sfRenderWindow_setFramerateLimit(window, 150);

	sfTexture* texture = sfTexture_createFromFile("res/test.png", NULL);
	sfSprite* sprite = sfSprite_create();
	sfSprite_setTexture(sprite, texture, sfFalse);

	while (sfRenderWindow_isOpen(window))
	{
		sfEvent ev;
		while (sfRenderWindow_pollEvent(window, &ev))
		{
			if (ev.type == sfEvtClosed)
			{
				sfRenderWindow_close(window);
			}
		}
		
		sfRenderWindow_clear(window, sfBlack);
		sfRenderWindow_drawSprite(window, sprite, NULL);
		sfRenderWindow_display(window);
	}
	
	sfSprite_destroy(sprite);
	sfTexture_destroy(texture);
	sfRenderWindow_destroy(window);
}
