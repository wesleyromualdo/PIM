<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\SistemaBase;

class Sistema extends SistemaBase {
 
    public function scopeAtivo($query) {
        return $query->where('sisstatus', 'A')->orderBy('sisdsc', 'asc');
    }

    /**
     * recupera os sistemas em Laravel em formato para o combo
     * @return mixed
     */
    public static function recuperaSistemasCombo(){
        $ilike = "%laravel%";

        return Sistema::where([
            ['sisstatus','=','A'],
            ['paginainicial','ilike',$ilike]
        ])->orderBy('sisdsc')->pluck('sisdsc', 'sisid');
    }

    public function sanitize()
    {
        $this->usucpfanalista = str_replace(array('.', '-'),'',$this->usucpfanalista);
        $this->usucpfgestor = str_replace(array('.', '-'),'',$this->usucpfgestor);
        $this->sistel = str_replace(array('(',')', ' '),array('','','-'),$this->sistel);
        $this->sisfax = str_replace(array('(',')', ' '),array('','','-'),$this->sisfax);
    }

    public function setNull()
    {
//        $this->sisdsc = "'".$this->sisdsc."'";
//        $this->sisabrev = "'".$this->sisabrev."'";
//        $this->sisdiretorio = "'".$this->sisdiretorio."'";
//        $this->sisfinalidade = "'".$this->sisfinalidade."'";
//        $this->sisrelacionado = "'".$this->sisrelacionado."'";
//        $this->sispublico = "'".$this->sispublico."'";
//        $this->paginainicial = "'".$this->paginainicial."'";
//        $this->sisarquivo = "'".$this->sisarquivo."'";
//        $this->sisstatus = "'".$this->sisstatus."'";

        !$this->sisurl ? $this->sisurl = null:$this->sisurl = $this->sisurl;
        !$this->sisemail ? $this->sisemail = null:$this->sisurl = $this->sisemail;
        !$this->sistel ? $this->sistel = null:$this->sistel = $this->sistel;
        !$this->sisfax ? $this->sisfax = null:$this->sisfax = $this->sisfax;
        !$this->sisordemmenu ? $this->sisordemmenu = null:$this->sisordemmenu = $this->sisordemmenu;
        !$this->usucpfanalista ? $this->usucpfanalista = null:$this->usucpfanalista;
        !$this->usucpfgestor ? $this->usucpfgestor = null:$this->usucpfgestor;

    }
}
