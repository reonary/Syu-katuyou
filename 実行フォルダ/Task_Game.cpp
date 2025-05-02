#include "MyPG.h"
#include "MyGameMain.h"

//ゲーム本編
namespace Game
{
	//ゲーム情報
	DG::Image::SP imgBG, imgPlayer[2], imgHeart[5], 
		imgFruitApple,imgFruitLemon,imgFruitOrange,imgFruitStrawberry,imgFruitWatermelon,
		imgWantFruitApple, imgWantFruitLemon, imgWantFruitOrange, imgWantFruitStrawberry, imgWantFruitWatermelon,
		imgKago,imgTree,imgWant,imgWantCat;

	enum class State { Normal, Non };
	bool right = true;
	bool left = false;
	bool gameover = false;
	bool takeFruits = false;
	int score = 0;
	int popFruit = 0;
	struct Chara {
		State state = State::Non;
		ML::Box2D hitBase;
		int Hp = 0;
		int x = 0;
		int y = 0;
		int moveX = 0;
		int moveY = 0;
		int timeCnt = 0;
		bool flag = false;
		int pos = 0;
		int speed = 2;
		int tag = 0;                //０：リンゴ、１：レモン、２：オレンジ、３：イチゴ、４：スイカ
	};
	Chara wantFruit;
	Chara player;
	Chara heart[5];
	Chara kago;
	Chara tree;
	Chara want;
	Chara wantCat;
	Chara fruits[100];
	Chara BG;
	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		popFruit = 5;
		score = 0;
		BG.x = 0;
		BG.y = 0;
		imgBG = DG::Image::Create("./data/image/BG.png");//背景
		imgTree = DG::Image::Create("./data/image/tree.png");
		tree.x = 0;
		tree.y = 0;
		imgWant = DG::Image::Create("./data/image/want.png");
		want.x = 20;
		want.y = 180;
		imgWantCat = DG::Image::Create("./data/image/wantCat.png");
		wantCat.x = -20;
		wantCat.y = 240;
		imgPlayer[0] = DG::Image::Create("./data/image/playerLeft.png");
		imgPlayer[1] = DG::Image::Create("./data/image/playerRight.png");
		gameover = false;
		player.state = State::Normal;
		player.Hp = 5;
		player.x = 240;
		player.y = 240;
		player.moveX = player.speed;
		player.moveY = player.speed;
		player.timeCnt = 0;
		player.flag = false;
		player.pos = 0;
		player.hitBase = ML::Box2D(-30, -30, 60, 60);
		//カゴ
		kago.x = player.x;
		kago.y = player.y - 10;
		kago.state = State::Normal;
		kago.hitBase = ML::Box2D(-10, -12, 20, 24);
		imgKago = DG::Image::Create("./data/image/kago.png");
		//果物
		for (int i = 0; i < popFruit; i++) {
			fruits[i].state = State::Non;
			fruits[i].timeCnt = 0;
			fruits[i].flag = false;
			fruits[i].x = 200;
			fruits[i].y = -50;
			fruits[i].tag = 0;
			fruits[i].speed = 2;
			fruits[i].moveY = fruits[i].speed;
			fruits[i].hitBase = ML::Box2D(-5, -5, 10, 10);
		}
		imgFruitApple = DG::Image::Create("./data/image/apple.png");
		imgFruitLemon = DG::Image::Create("./data/image/lemon.png");
		imgFruitOrange = DG::Image::Create("./data/image/orange.png");
		imgFruitStrawberry = DG::Image::Create("./data/image/strawberry.png");
		imgFruitWatermelon = DG::Image::Create("./data/image/waterMelon.png");
		//要求果物
		imgWantFruitApple = DG::Image::Create("./data/image/apple.png");
		imgWantFruitLemon = DG::Image::Create("./data/image/lemon.png");
		imgWantFruitOrange = DG::Image::Create("./data/image/orange.png");
		imgWantFruitStrawberry = DG::Image::Create("./data/image/strawberry.png");
		imgWantFruitWatermelon = DG::Image::Create("./data/image/waterMelon.png");
		wantFruit.x = 23;
		wantFruit.y = 183;
	    wantFruit.tag = 0;
		wantFruit.state = State::Non;
		wantFruit.timeCnt = 0;
		//体力のハート
		for (int i = 0; i < 5; i++) {
			imgHeart[i] = DG::Image::Create("./data/image/heart.png");
			heart[i].x = 15;
			heart[i].y = (i + 1) * 20;
			heart[i].state = State::Normal;
		}
		//シーン切り替えを分かりやすくするために入れた
		ge->dgi->EffectState().param.bgColor = ML::Color(1, 0, 1, 0);
	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		imgBG.reset(), 
		imgFruitApple.reset(), 
		imgFruitLemon.reset(), 
		imgFruitOrange.reset(), 
		imgFruitStrawberry.reset(), 
		imgFruitWatermelon.reset(),
		imgKago.reset(), 
		imgTree.reset(), 
		imgWantCat.reset(),
		imgWant.reset();
		for (int i = 0; i < 2; i++) {
			imgPlayer[i].reset();
		}
		for (int i = 0; i < 5; i++) {
			imgHeart[i].reset();
		}
			
	}
	//-----------------------------------------------------------------------------
	//実行処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		popFruit = 5;
		auto inp = ge->in1->GetState();//プレイヤ
		if (player.Hp <= 0) {
			gameover = true;
		}
		
		if (player.x >= 100 && true == inp.LStick.BL.on && player.state == State::Normal) { left = true, right = false, player.x -= 3; }
		if (player.x <= 480 - 32 && true == inp.LStick.BR.on && player.state == State::Normal) { right = true, left = false, player.x += 3; }
		if (player.state == State::Normal) {                             //敵との当たり判定
			ML::Box2D me = kago.hitBase.OffsetCopy(kago.x, kago.y+5);
			for (int f = 0; f < popFruit;f++)
			{
				if (fruits[f].state == State::Normal) {
					ML::Box2D fruit = fruits[f].hitBase.OffsetCopy(fruits[f].x, fruits[f].y);
					
					if (fruit.Hit(me) == true) {
						if (fruits[f].tag == wantFruit.tag) { //接触判定 正解
							score++;
						}
						else {
							player.Hp--;
							for (int i = 0; i < 5; i++) {
								if(player.Hp<=i)
								heart[i].state = State::Non;
							}
						}
						takeFruits = true;
						fruits[f].state = State::Non;
						fruits[f].timeCnt = 0;
						
						
					}
				}
			}
		}
		//カゴ
		kago.x = player.x;
		kago.y = player.y - 35;
		//果物
		for (int i = 0; i < popFruit; i++) {
			fruits[i].timeCnt++;
			if (fruits[i].y > 330) {                         //画面外に出たら果物の場所を画面外上に移動
				fruits[i].state = State::Non;
			}
			if (fruits[i].state == State::Non) {
				fruits[i].tag = rand() % popFruit;
				fruits[i].x = rand() % 340 + 120;
				fruits[i].y = -30;
				fruits[i].speed = rand() % 3 + 1;
				fruits[i].moveY = fruits[i].speed;
				if (fruits[i].timeCnt >= i * 100 + rand() % 101) {
					fruits[i].timeCnt = 0;
					fruits[i].state = State::Normal;
				}
			}
			if(fruits[i].state == State::Normal)
			{
				fruits[i].y += fruits[i].moveY;                     //果物が存在している時、下降する
			}
		}
		//要求果物
		if (wantFruit.state == State::Non)
		{
			wantFruit.tag = rand() % popFruit;
			wantFruit.state = State::Normal;
		}
		else if (wantFruit.state == State::Normal) {
				wantFruit.timeCnt++;
				if (wantFruit.timeCnt >= 250) {
					wantFruit.timeCnt = 0;
					wantFruit.state = State::Non;
				}
			}
			

		TaskFlag rtv = TaskFlag::Game;
		if (true == gameover) {
			rtv = TaskFlag::Ending;
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理
	//-----------------------------------------------------------------------------
	void Render()
	{
		
		//背景描画
		ML::Box2D  drawBG(0, 0, 480, 330);
		drawBG.Offset(BG.x,BG.y );
		ML::Box2D  srcBG(0, 0, 1600, 900);
		imgBG->Draw(drawBG, srcBG);
		//木
		ML::Box2D drawTree(-100, -100, 400, 400);
		drawTree.Offset(tree.x, tree.y);
		ML::Box2D srcTree(120, 0, 623, 1222);
		imgTree->Draw(drawTree, srcTree);
		//ハート描画
		
		for (int i = 0; i < 5; i++) {
			if (heart[i].state == State::Normal) {
				ML::Box2D drawHeart(0, 0, 30, 30);
				ML::Box2D srcHeart(0, 0, 500, 500);
				drawHeart.Offset(heart[i].x, heart[i].y);
				imgHeart[i]->Draw(drawHeart, srcHeart);
			}
		}
		//吹き出し
		ML::Box2D drawWant(0, 0, 80, 80);
		drawWant.Offset(want.x, want.y);
		ML::Box2D srcWant(0, 0, 626, 609);
		imgWant->Draw(drawWant, srcWant);
		//果物要求ネコ
		ML::Box2D drawWantCat(0, 0, 50, 50);
		drawWantCat.Offset(wantCat.x, wantCat.y);
		ML::Box2D srcWantCat(0, 0, 772, 769);
		imgWantCat->Draw(drawWantCat, srcWantCat);
		//プレイヤ描画
		if (right == true) {
			ML::Box2D drawPlayerRight(-30, -30, 60, 60);
			drawPlayerRight.Offset(player.x, player.y);
			ML::Box2D srcPlayerRight(0, 0, 500, 500);
			imgPlayer[1]->Draw(drawPlayerRight, srcPlayerRight);
		}
		else{
			ML::Box2D drawPlayerLeft(-30, -30, 60, 60);
			drawPlayerLeft.Offset(player.x, player.y);
			ML::Box2D srcPlayerLeft(0, 0, 500, 500);
			imgPlayer[0]->Draw(drawPlayerLeft, srcPlayerLeft);
		}
		//カゴ描画
		ML::Box2D drawKago(-12, -15, 24, 30);
		drawKago.Offset(kago.x,kago.y);
		ML::Box2D srcKago(0, 0, 1200, 900);
		imgKago->Draw(drawKago, srcKago);
		//果物描画
		
		for (int i = 0; i < popFruit; i++) {
			if (fruits[i].state == State::Normal) {
				//リンゴ
				ML::Box2D drawApple(-15, -11, 30, 23);
				ML::Box2D srcApple(0, 0, 800, 600);
				//レモン
				ML::Box2D drawLemon(-15, -11, 30, 23);
				ML::Box2D srcLemon(0, 0, 800, 600);
				//オレンジ
				ML::Box2D drawOrange(-15, -11, 30, 23);
				 ML::Box2D srcOrange(0, 0, 800, 600);
				//イチゴ
				 ML::Box2D drawStrawberry(-15, -11, 30, 23);
				 ML::Box2D srcStrawberry(0, 0, 800, 600);
				//スイカ
				 ML::Box2D drawWatermelon(-15, -11, 30, 23);
				 ML::Box2D srcWatermelon(0, 0, 800, 600);
				switch (fruits[i].tag) {
				case 0:   //リンゴ
				{
				drawApple.Offset(fruits[i].x, fruits[i].y);
				imgFruitApple->Draw(drawApple, srcApple);
				break;
				}
				case 1:    //レモン
				{
				drawLemon.Offset(fruits[i].x, fruits[i].y);
				imgFruitLemon->Draw(drawLemon, srcLemon);
				break; 
				}
				case 2:   //オレンジ
				drawOrange.Offset(fruits[i].x, fruits[i].y);
				imgFruitOrange->Draw(drawOrange, srcOrange);
				break;
				case 3:    //イチゴ
				drawStrawberry.Offset(fruits[i].x, fruits[i].y);
				imgFruitStrawberry->Draw(drawStrawberry, srcStrawberry);
				break;
				case 4:    //スイカ
					drawWatermelon.Offset(fruits[i].x, fruits[i].y);
					imgFruitWatermelon->Draw(drawWatermelon, srcWatermelon);
					break;
				}
			}
		}
		//要求果物
		if ( wantFruit.state == State::Normal) {
				switch (wantFruit.tag) {
				case 0:   //リンゴ
				{
					ML::Box2D drawWantApple(0, 0, 80, 60);
					drawWantApple.Offset(wantFruit.x, wantFruit.y);
					ML::Box2D srcApple(0, 0, 800, 600);
					imgWantFruitApple->Draw(drawWantApple, srcApple);
				}
					break;
				case 1:    //レモン
				{
					ML::Box2D drawWantLemon(0, 0, 80, 60);
					drawWantLemon.Offset(wantFruit.x, wantFruit.y);
					ML::Box2D srcLemon(0, 0, 800, 600);
					imgWantFruitLemon->Draw(drawWantLemon, srcLemon);
				}
					break;
				case 2:   //オレンジ
				{
					ML::Box2D drawWantOrange(0, 0, 80, 60);
					drawWantOrange.Offset(wantFruit.x, wantFruit.y);
					ML::Box2D srcOrange(0, 0, 800, 600);
					imgWantFruitOrange->Draw(drawWantOrange, srcOrange);
				}
					break;
				case 3:    //イチゴ
				{
					ML::Box2D drawWantStrawberry(0, 0, 80, 60);
					drawWantStrawberry.Offset(wantFruit.x, wantFruit.y);
					ML::Box2D srcStrawberry(0, 0, 800, 600);
					imgWantFruitStrawberry->Draw(drawWantStrawberry, srcStrawberry);
				}
					break;
				case 4:    //スイカ
				{
					ML::Box2D drawWantWatermelon(0, 0, 80, 60);
					drawWantWatermelon.Offset(wantFruit.x, wantFruit.y);
					ML::Box2D srcWatermelon(0, 0, 800, 600);
					imgWantFruitWatermelon->Draw(drawWantWatermelon, srcWatermelon);
				}
					break;
			}
		}
	}
}