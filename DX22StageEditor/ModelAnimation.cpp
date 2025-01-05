/* ========================================
	DX22Base/
	------------------------------------
	アニメーションモデル(アニメーション処理)用cpp
	------------------------------------
	ModelAnimation.cpp
========================================== */


// =============== インクルード ===================
#include "ModelAnime.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#if _MSC_VER >= 1930
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "assimp-vc143-mt.lib")
#endif
#elif _MSC_VER >= 1920
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif
#elif _MSC_VER >= 1910
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc141-mtd.lib")
#else
#pragma comment(lib, "assimp-vc141-mt.lib")
#endif
#endif



/* ========================================
	アニメーション追加関数
	-------------------------------------
	内容：アニメーションを読込、追加する
	-------------------------------------
	引数：読み込むアニメーションファイルへのパス
	-------------------------------------
	戻値：内部で割り当てられたアニメーション番号
=========================================== */
ModelAnime::AnimeNo ModelAnime::AddAnimation(const char* file)
{
#ifdef _DEBUG
	m_sErrorMessage = "";
#endif

	
	// assimpの設定
	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_FlipUVs;
	if (m_LoadFlip == E_Flip::XFlip)  flag |= aiProcess_MakeLeftHanded;

	// assimpで読み込み
	const aiScene* pScene = importer.ReadFile(file, flag);
	if (!pScene)
	{
#ifdef _DEBUG
		m_sErrorMessage += importer.GetErrorString();
#endif
		return ANIME_NONE;
	}

	// アニメーションチェック
	if (!pScene->HasAnimations())
	{
#ifdef _DEBUG
		m_sErrorMessage += "no animation.";
#endif
		return ANIME_NONE;
	}

	// アニメーション作成
	MakeAnimation(pScene);

	
	m_AnimeList.back().sName = GetFBXDataName(file);

	// アニメ番号を返す
	return static_cast<AnimeNo>(m_AnimeList.size() - 1);
}



/* ========================================
	アニメーション再生関数
	-------------------------------------
	内容：指定したアニメーションを再生する
	-------------------------------------
	引数1：再生するアニメーション番号
	引数2：ループ再生フラグ
	引数3：再生速度
=========================================== */
void ModelAnime::Play(AnimeNo no, bool loop, float speed)
{
	// 再生チェック
	if (!AnimeNoCheck(no)) { return; }
	if (m_nActiveNo == no) { return; }

	// 合成アニメーションかチェック
	if (no != PARAMETRIC_ANIME)
	{
		// 通常の初期化
		InitAnime(no);
		m_AnimeList[no].isLoop = loop;
		m_AnimeList[no].speed = speed;
	}
	else
	{
		// 合成アニメーションの元になっているアニメーションを初期化
		InitAnime(m_nParametricAnimeNos[0]);
		InitAnime(m_nParametricAnimeNos[1]);
		m_AnimeList[m_nParametricAnimeNos[0]].isLoop = loop;
		m_AnimeList[m_nParametricAnimeNos[1]].isLoop = loop;
	}

	// 再生アニメーションの設定
	m_nActiveNo = no;
}


/* ========================================
	アニメーション再生(ブレンド)関数
	-------------------------------------
	内容：現在再生中のアニメから指定したアニメに
		　滑らかに遷移する
	-------------------------------------
	引数1：再生するアニメーション番号
	引数2：ブレンド時間
	引数3：ループ再生フラグ
	引数4：再生速度
=========================================== */
void ModelAnime::PlayBlend(AnimeNo no, float blendTime, bool loop, float speed)
{
	// 再生チェック
	if (!AnimeNoCheck(no)) { return; }

	// 合成アニメーションかチェック
	if (no != PARAMETRIC_ANIME)
	{
		InitAnime(no);
		m_AnimeList[no].isLoop = loop;
		m_AnimeList[no].speed = speed;
	}
	else
	{
		// 合成アニメーションの元になっているアニメーションを初期化
		InitAnime(m_nParametricAnimeNos[0]);
		InitAnime(m_nParametricAnimeNos[1]);
		m_AnimeList[m_nParametricAnimeNos[0]].isLoop = loop;
		m_AnimeList[m_nParametricAnimeNos[1]].isLoop = loop;
	}

	// ブレンドの設定
	m_fBlendTime		= 0.0f;
	m_fBlendTimeTotal	= blendTime;
	m_nBlendNo			= no;
}


/* ========================================
	合成元アニメーション設定関数
	-------------------------------------
	内容：合成元アニメーションを設定する
		　※合成アニメーション再生はPARAMETRIC_ANIMEを指定する
	-------------------------------------
	引数1：合成元アニメーション1
	引数2：合成元アニメーション2
=========================================== */
void ModelAnime::SetParametric(AnimeNo no1, AnimeNo no2)
{
	// アニメーションチェック
	if (!AnimeNoCheck(no1)) { return; }
	if (!AnimeNoCheck(no2)) { return; }

	// 合成設定
	m_nParametricAnimeNos[0] = no1;
	m_nParametricAnimeNos[1] = no2;
	SetParametricBlend(0.0f);
}


/* ========================================
	合成アニメーション割合設定関数
	-------------------------------------
	内容：合成アニメーションの合成割合を設定
	-------------------------------------
	引数：合成割合
=========================================== */
void ModelAnime::SetParametricBlend(float blendRate)
{
	// 合成元アニメが設定されているか確認
	if (m_nParametricAnimeNos[0] == ANIME_NONE || m_nParametricAnimeNos[1] == ANIME_NONE) return;

	// 合成割合設定
	m_fParametricBlendRatio = blendRate;

	// 割合に基づいてアニメーションの再生速度を設定
	T_Animation& anime1 = m_AnimeList[m_nParametricAnimeNos[0]];
	T_Animation& anime2 = m_AnimeList[m_nParametricAnimeNos[1]];
	float blendTotalTime = anime1.totalTime * (1.0f - m_fParametricBlendRatio) + anime2.totalTime * m_fParametricBlendRatio;
	anime1.speed = anime1.totalTime / blendTotalTime;
	anime2.speed = anime2.totalTime / blendTotalTime;
}


/* ========================================
	アニメーション再生時間位置変更関数
	-------------------------------------
	内容：指定したアニメーションの再生時間位置を変更
	-------------------------------------
	引数1：変更するアニメ
	引数2：再生時間位置
=========================================== */
void ModelAnime::SetAnimationTime(AnimeNo no, float time)
{
	// アニメーションチェック
	if (!AnimeNoCheck(no)) { return; }

	// 再生時間変更
	T_Animation& anime = m_AnimeList[no];
	anime.nowTime = time;

	// 再生時間がアニメーションの総時間を超えていたら、ループさせる
	while (anime.nowTime >= anime.totalTime)
	{
		anime.nowTime -= anime.totalTime;
	}
}

/* ========================================
	アニメーション再生割合位置変更関数
	-------------------------------------
	内容：指定したアニメーションの再生割合位置を変更
	-------------------------------------
	引数1：変更するアニメ
	引数2：再生割合位置
=========================================== */
void ModelAnime::SetAnimationTimeRatio(AnimeNo no, float rate)
{
	// アニメーションチェック
	if (!AnimeNoCheck(no)) { return; }

	// 再生時間変更
	T_Animation& anime = m_AnimeList[no];
	anime.nowTime = anime.totalTime * rate;

	// 再生時間がアニメーションの総時間を超えていたら、ループさせる
	while (anime.nowTime >= anime.totalTime)
	{
		anime.nowTime -= anime.totalTime;
	}
}

/* ========================================
	アニメーション再生時間取得関数
	-------------------------------------
	内容：アニメーションの現在再生時間を取得
	-------------------------------------
	引数：取得するアニメ番号
	-------------------------------------
	戻値：再生時間
=========================================== */
float ModelAnime::GetAnimeNowTime(AnimeNo no)
{
	// アニメーションチェック
	if (!AnimeNoCheck(no)) { return 0.0f; }

	T_Animation& anime = m_AnimeList[no];

	return anime.nowTime;
}

/* ========================================
	アニメーショントータル再生時間取得関数
	-------------------------------------
	内容：アニメーションのトータル再生時間を取得
	-------------------------------------
	引数：取得するアニメ番号
	-------------------------------------
	戻値：トータル時間
=========================================== */
float ModelAnime::GetAnimeTotalTime(AnimeNo no)
{
	// アニメーションチェック
	if (!AnimeNoCheck(no)) { return 0.0f; }

	T_Animation& anime = m_AnimeList[no];

	return anime.totalTime;
}

/* ========================================
	アニメーション再生割合取得関数
	-------------------------------------
	内容：アニメーションの再生割合を取得
	-------------------------------------
	引数：取得するアニメ番号
	-------------------------------------
	戻値：再生割合
=========================================== */
float ModelAnime::GetAnimePlaybackRatio(AnimeNo no)
{
	// アニメーションチェック
	if (!AnimeNoCheck(no)) { return 0.0f; }

	T_Animation& anime = m_AnimeList[no];

	return anime.nowTime / anime.totalTime;
}


/* ========================================
	アニメーション再生速度変更関数
	-------------------------------------
	内容：指定したアニメーションの再生速度を変更する
	-------------------------------------
	引数1：変更するアニメ番号
	引数2：新しい再生速度
=========================================== */
void ModelAnime::SetAnimationSpeed(AnimeNo no, float speed)
{
	// アニメーションチェック
	if (!AnimeNoCheck(no)) { return; }

	m_AnimeList[no].speed = speed;
}

/* ========================================
	再生フラグ取得関数
	-------------------------------------
	内容：指定したアニメーションが再生中かどうかを取得
	-------------------------------------
	引数1：調べるアニメ番号
	引数2：true = 再生中 / false = 停止中
=========================================== */
bool ModelAnime::IsPlay(AnimeNo no)
{
	// アニメーションチェック
	if (!AnimeNoCheck(no)) { return false; }

	// パラメトリックは合成元のアニメを基準に判断
	if (no == PARAMETRIC_ANIME) { no = m_nParametricAnimeNos[0]; }

	// 再生時間の判定
	if (m_AnimeList[no].totalTime < m_AnimeList[no].nowTime) { return false; }

	// それぞれの再生番号に設定されているか確認
	if (m_nActiveNo == no) { return true; }
	if (m_nBlendNo == no) { return true; }
	if (m_nActiveNo == PARAMETRIC_ANIME || m_nBlendNo == PARAMETRIC_ANIME)
	{
		if (m_nParametricAnimeNos[0] == no) { return true; }
		if (m_nParametricAnimeNos[1] == no) { return true; }
	}

	// 再生中でない
	return false;
}


/* ========================================
	アニメ番号チェック関数
	-------------------------------------
	内容：指定したアニメ番号が正しいかどうかを確認
	-------------------------------------
	引数：確認するアニメ番号
	-------------------------------------
	戻値：true = 正常 / false = 異常
=========================================== */
bool ModelAnime::AnimeNoCheck(AnimeNo no)
{
	// パラメトリックアニメーション確認
	if (no == PARAMETRIC_ANIME)
	{
		// パラメトリックのアニメーションが両方正しく設定されているか
		return
			m_nParametricAnimeNos[0] != ANIME_NONE &&
			m_nParametricAnimeNos[1] != ANIME_NONE;
	}
	else
	{
		// 問題ないアニメーション番号かどうか
		return 0 <= no && no < m_AnimeList.size();
	}
}


/* ========================================
	アニメーション初期化関数
	-------------------------------------
	内容：指定したアニメーションを初期化する
	-------------------------------------
	引数：アニメ番号
=========================================== */
void ModelAnime::InitAnime(AnimeNo no)
{
	// アニメの設定なし、パラメトリックで設定されているなら初期化しない
	if (no == ANIME_NONE || no == PARAMETRIC_ANIME) { return; }

	T_Animation& anime = m_AnimeList[no];
	anime.nowTime = 0.0f;
	anime.speed = 1.0f;
	anime.isLoop = false;
}


/* ========================================
	アニメーション計算関数
	-------------------------------------
	内容：指定したアニメーションの姿勢を計算する
	-------------------------------------
	引数1：アニメーションの種類
	引数2：アニメーション番号
=========================================== */
void ModelAnime::CalcAnime(E_AnimeTransform kind, AnimeNo no)
{
	T_Animation& anime = m_AnimeList[no];
	Channels::iterator channelIt = anime.channels.begin();

	while (channelIt != anime.channels.end())
	{
		// 一致するボーンがなければスキップ
		Timeline& timeline = channelIt->timeline;
		if (channelIt->index == INDEX_NONE || timeline.empty())
		{
			++channelIt;
			continue;
		}

		//--- 該当ノードの姿勢をアニメーションで更新
		T_Transform& transform = m_NodeTransform[kind][channelIt->index];
		if (timeline.size() <= 1)
		{
			// キーが一つしかないので値をそのまま使用
			transform = channelIt->timeline[0];
		}
		else
		{
			Timeline::iterator startIt = timeline.begin();
			if (anime.nowTime <= startIt->first)
			{
				// 先頭キーよりも前の時間なら、先頭の値を使用
				transform = startIt->second;
			}
			else if (timeline.rbegin()->first <= anime.nowTime)
			{
				// 最終キーよりも後の時間なら、最後の値を使用
				transform = timeline.rbegin()->second;
			}
			else
			{
				// 指定された時間を挟む2つのキーから、補間された値を計算
				Timeline::iterator nextIt = timeline.upper_bound(anime.nowTime);
				startIt = nextIt;
				--startIt;
				float rate = (anime.nowTime - startIt->first) / (nextIt->first - startIt->first);
				LerpTransform(&transform, startIt->second, nextIt->second, rate);
			}
		}

		++channelIt;
	}
}


/* ========================================
	アニメーション更新関数
	-------------------------------------
	内容：指定したアニメーションの再生時間を更新
	-------------------------------------
	引数1：アニメーション番号
	引数2：経過時間
=========================================== */
void ModelAnime::UpdateAnime(AnimeNo no, float tick)
{
	if (no == PARAMETRIC_ANIME) { return; }

	T_Animation& anime = m_AnimeList[no];
	anime.nowTime += anime.speed * tick;
	if (anime.isLoop)
	{
		while (anime.nowTime >= anime.totalTime)
		{
			anime.nowTime -= anime.totalTime;
		}
	}
}


/* ========================================
	ボーン計算関数
	-------------------------------------
	内容：ボーンの姿勢行列を計算
	-------------------------------------
	引数1：ボーン番号
	引数2：親ノードの姿勢行列
=========================================== */
void ModelAnime::CalcBones(NodeIndex index, const DirectX::XMMATRIX parent)
{
	//--- アニメーションごとのパラメータを合成
	T_Transform transform;
	// パラメトリック
	if (m_nActiveNo == PARAMETRIC_ANIME || m_nBlendNo == PARAMETRIC_ANIME)
	{
		LerpTransform(&transform, m_NodeTransform[PARAMETRIC0][index], m_NodeTransform[PARAMETRIC1][index], m_fParametricBlendRatio);
		if (m_nActiveNo == PARAMETRIC_ANIME)
		{
			m_NodeTransform[MAIN][index] = transform;
		}
		if (m_nBlendNo == PARAMETRIC_ANIME)
		{
			m_NodeTransform[BLEND][index] = transform;
		}
	}
	// ブレンドアニメ
	if (m_nBlendNo != ANIME_NONE)
	{
		LerpTransform(&transform, m_NodeTransform[MAIN][index], m_NodeTransform[BLEND][index], m_fBlendTime / m_fBlendTimeTotal);
	}
	else
	{
		// メインアニメのみ
		transform = m_NodeTransform[MAIN][index];
	}

	// 該当ノードの姿勢行列を計算
	T_Node& node = m_NodeList[index];
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&transform.position));
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&transform.quaternion));
	DirectX::XMMATRIX S = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&transform.scale));
	node.mat = (S * R * T) * parent;

	// 子要素の姿勢を更新
	Children::iterator it = node.children.begin();
	while (it != node.children.end())
	{
		CalcBones(*it, node.mat);
		++it;
	}
}


/* ========================================
	補間関数
	-------------------------------------
	内容：2つの姿勢を補間
	-------------------------------------
	引数1：補間結果を格納する変数
	引数2：補間元A
	引数3：補間元B
	引数4：補間率
=========================================== */
void ModelAnime::LerpTransform(T_Transform* pOut, const T_Transform& a, const T_Transform& b, float rate)
{
	DirectX::XMVECTOR vec[][2] = {
		{ DirectX::XMLoadFloat3(&a.position),	DirectX::XMLoadFloat3(&b.position) },
		{ DirectX::XMLoadFloat4(&a.quaternion),	DirectX::XMLoadFloat4(&b.quaternion) },
		{ DirectX::XMLoadFloat3(&a.scale),		DirectX::XMLoadFloat3(&b.scale) },
	};
	for (int i = 0; i < 3; ++i)
	{
		vec[i][0] = DirectX::XMVectorLerp(vec[i][0], vec[i][1], rate);
	}
	DirectX::XMStoreFloat3(&pOut->position, vec[0][0]);
	DirectX::XMStoreFloat4(&pOut->quaternion, vec[1][0]);
	DirectX::XMStoreFloat3(&pOut->scale, vec[2][0]);
}



/* ========================================
	ゲッター(再生中アニメ番号)関数
	-------------------------------------
	戻値：int	再生中アニメ番号
=========================================== */
ModelAnime::AnimeNo ModelAnime::GetPlayNo()
{
	return m_nActiveNo;
}

/* ========================================
	ゲッター(再生中ブレンドアニメ番号)関数
	-------------------------------------
	戻値：int	再生中ブレンドアニメ番号
=========================================== */
ModelAnime::AnimeNo ModelAnime::GetBlendNo()
{
	return m_nBlendNo;
}

/* ========================================
	ゲッター(モデル名)関数
	-------------------------------------
	戻値：std::string	モデル名
=========================================== */
std::string ModelAnime::GetModelName()
{
	return m_sModelName;
}

#ifdef _DEBUG

/* ========================================
	ゲッター(エラーメッセージ)関数
	-------------------------------------
	戻値：std::string	エラーメッセージ
=========================================== */
std::string ModelAnime::GetError()
{
	return m_sErrorMessage;
}


/* ========================================
	ボーンデバッグ描画関数
	-------------------------------------
	内容：ボーンを描画する
=========================================== */
void ModelAnime::DrawBone(Vector3<float> vPos, Quaternion qRot, Vector3<float> vScale)
{
	m_pBoneLine->SetPos(vPos);
	m_pBoneLine->SetRotation(qRot);
	m_pBoneLine->SetScale(vScale);

	m_nBoneCnt = 0;
	// 再帰処理
	std::function<void(int, DirectX::XMFLOAT3)> FuncDrawBone =
		[&FuncDrawBone, this](int idx, DirectX::XMFLOAT3 parent)
	{
		// 親ノードから現在位置まで描画
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, DirectX::XMVector3TransformCoord(DirectX::XMVectorZero(), m_NodeList[idx].mat));

		Vector3<float> start, end;
		start = parent;
		end = pos;

		m_pBoneLine->UpdateLine(m_nBoneCnt, start, end, ColorVec3::RED);
		++m_nBoneCnt;
		// 子ノードの描画
		auto it = m_NodeList[idx].children.begin();
		while (it != m_NodeList[idx].children.end())
		{
			FuncDrawBone(*it, pos);
			++it;
		}
	};

	// 描画実行
	FuncDrawBone(0, DirectX::XMFLOAT3());
	m_pBoneLine->Draw();
}

#endif