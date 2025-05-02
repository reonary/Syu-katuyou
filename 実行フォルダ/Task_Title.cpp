#include "MyPG.h"
#include "MyGameMain.h"

//タイトル画面
namespace Title
{
	//ゲーム情報
	int					logoPosY;
	DG::Image::SP       imgTitle;
	enum class State { Normal, Non };
	struct Chara {
		State state = State::Non;
		int x = 0;
		int y = 0;
		int timeCnt = 0;
	};

	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		imgTitle = DG::Image::Create("./data/image/BG.png");
		logoPosY = 0; //タイトル画像の初期位置を画面外に配置する

		//シーン切り替えを分かりやすくするために入れた
		ge->dgi->EffectState().param.bgColor = ML::Color(1, 0, 0, 1);
	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		imgTitle.reset( );
	}
	//-----------------------------------------------------------------------------
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto inp = ge->in1->GetState();
		logoPosY += 9;
		if (logoPosY  >  0) { logoPosY = 0; }
		//logoPosY = min(logoPosY + 9, 0);

		TaskFlag rtv = TaskFlag::Title;//取りあえず現在のタスクを指定
		if (logoPosY >= 0) {
			if (true == inp.ST.down) {
				rtv = TaskFlag::Game;
			}
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理
	//-----------------------------------------------------------------------------
	void Render()
	{
		ML::Box2D  drawTitle(0, 0, 480, 270);
		drawTitle.Offset(0, logoPosY);
		ML::Box2D  srcTitle(0, 0, 1600, 900);

		imgTitle->Draw(drawTitle, srcTitle);
	}
}