<?php

namespace App\Entities;

use Illuminate\Database\Eloquent\Model;
use Illuminate\Support\Facades\Request;

class Arquivo extends Model {

    protected $fillable = ['arqid', 'arqs', 'sisid', 'arqnome', 'arqdescricao', 'arqextensao', 'arqtipo', 'arqtamanho', 'arqdata', 'arqhora', 'arqstatus', 'usucpf'];
    protected $guarded = ['arqid', 'sisid'];
    protected $primaryKey = 'arqid';
    protected $table = 'public.arquivo';
    public $timestamps = false;
    public $arqs = [];
    public $strDir = '../../arquivos/';

    public function scopeAtivo($query) {
        return $query->where('arqstatus', 1);
    }

    /**
     * @param $model
     * @param $arquivo
     * @param null $nomeModulo
     * @param bool $booMoverArquivo
     */
    private function salvaArquivo($model, $arquivo, $key, $nomeModulo = null, $booMoverArquivo = true)
    {       
        if (is_object($arquivo)) {
            $arq = new Arquivo();
            $arq->arqnome = $model->getKey() . date('His') . '_' . $arquivo->getClientOriginalName();
            $arq->arqdescricao = $arquivo->getClientOriginalName();
            $arq->arqextensao = $arquivo->getClientOriginalExtension();
            $arq->arqtipo = $arquivo->getClientMimeType();
            $arq->arqtamanho = $arquivo->getClientSize();
            $arq->arqdata = date('Y-m-d');
            $arq->arqhora = date('H:i:s');
            $arq->arqstatus = 'A';
            $arq->usucpf = Request::session()->get('usucpf');
            $arq->sisid = Request::session()->get('sisid');
            $arq->save();
            
            if (!is_null($key)) {
                $this->arqs[$key] = $arq;        
                if ($booMoverArquivo && !empty($this->arqs[$key])) {
                    $this->moverArquivo($arquivo, $this->arqs[$key], $nomeModulo);
                }
            } else {
                $this->arqs = $arq;
                if ($booMoverArquivo && !empty($this->arqs)) {
                    $this->moverArquivo($arquivo, $this->arqs, $nomeModulo);
                }
            }
        } else {
            $this->arqs[] = $key;
        }
                
        return $this->arqs;
    }

    /**
     * @param $model
     * @param $arrayModelUpload
     * @param null $nomeModulo
     * @param bool $booMoverArquivo
     */
    public function salvaArquivos($model, $arrayModelUpload, $nomeModulo = null, $booMoverArquivo = true)
    {
        
        if ($arrayModelUpload) {
            if (is_array($arrayModelUpload) && count($arrayModelUpload) >= 1) {
                foreach ($arrayModelUpload as $key => $arquivo) {
                    $this->salvaArquivo($model, $arquivo, $key, $nomeModulo, $booMoverArquivo);
                }
            } else {
                $this->salvaArquivo($model, $arrayModelUpload, null, $nomeModulo, $booMoverArquivo);
            }
        } else {
            $this->arqs = [];
        }
    }

    /**
     * @param $arquivo
     * @param $arq
     * @param null $nomeModulo
     * @param null $url
     */
    public function moverArquivo($arquivo, $arq, $nomeModulo = null, $url = null)
    {
        $url = !empty($url) ? $url : $this->criarPasta($arq->arqid, $nomeModulo);
        if ($arquivo && is_object($arquivo)) {
            $arquivo->move($url, $arq->arqid);
        }

    }

    /**
     * @param $arqid
     * @param null $nomeModulo
     * @return string
     */
    public function criarPasta($arqid, $nomeModulo = null)
    {
        $nomeModulo = $nomeModulo ? $nomeModulo : $_SESSION['sisdiretorio'];

        if ($nomeModulo) {
            $url = storage_path($this->strDir . $nomeModulo);

            if (!is_dir($url)) {
                mkdir($url, 0777);
            }

            $url = storage_path($this->strDir . $nomeModulo . '/' . floor($arqid/1000));

            if (!is_dir($url)) {
                mkdir($url, 0777);
            }
        } else {
            $url = storage_path($this->strDir);
        }
        return $url;
    }

    /**
     * @param $arqid
     * @param $arqdescricao
     * @param null $nomeModulo
     */
    public function download($arqid, $arqdescricao, $nomeModulo = null)
    {
        $nomeModulo = $nomeModulo ? $nomeModulo : $_SESSION['sisdiretorio'];
        $arquivo = $this->strDir . $nomeModulo . '/' . floor($arqid/1000) . '/' . $arqid;

        $mime_type = "application/octet-stream"; # can be improved later
        $size      = filesize($arquivo);

        ob_start();
        header('Content-Description: File Transfer');
        header('Content-Type: ' . $mime_type);
        header('Content-Disposition: attachment; filename=' . $arqdescricao);
        header('Content-Length: ' . $size);
        readfile($arquivo);
        ob_end_flush();

    }

    public static function excluirArquivo($dados) {

    }
}
