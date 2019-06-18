<?php

class HistoricoSolicitacaoEmpenho extends cls_banco
{
    public $hserequest_id, $empid, $hsesistema, $lgwid;
    
    public function __construct()
    {
        parent::__construct();
    }
    
    public function __destruct()
    {
        if (isset($_SESSION['transacao'])) {
            pg_query($this->link, 'rollback; ');
            unset($_SESSION['transacao']);
        }
    }
    
    public function pegaRequest_ID()
    {
        return parent::pegaUm("SELECT coalesce(max(hserequest_id),0)+1 FROM par3.historicosolicitacaoempenho WHERE sisid = {$_SESSION['sisid']} AND hsesistema = {$this->hsesistema}");
    }
    
    public function gravarRequest_ID()
    {
        $this->hserequest_id = $this->pegaRequest_ID();
        $empid = ($this->empid ? $this->empid : 'null');
        $pagid = ($this->pagid ? $this->pagid : 'null');
        $lgwid = ($this->lgwid ? $this->lgwid : 'null');
        
        $sql = "INSERT INTO par3.historicosolicitacaoempenho(hserequest_id, empid, pagid, lgwid, usucpf, sisid, hsesistema, hsedata)
                VALUES({$this->hserequest_id}, {$empid}, {$pagid}, {$lgwid}, '{$_SESSION['usucpf']}', {$_SESSION['sisid']}, {$this->hsesistema}, now())";
        parent::executar($sql);
        parent::commit();
        
        return $this->hserequest_id;
    }
    
    public function alterar($hserequest_id)
    {
        $coluna = array();
        if (!empty($this->empid)) {
            $coluna[]= "empid = {$this->empid}";
        }
        if (!empty($this->pagid)) {
            $coluna[]= "pagid = {$this->pagid}";
        }
        if (!empty($this->lgwid)) {
            $coluna[]= "lgwid = {$this->lgwid}";
        }
        
        if ($coluna[0]) {
            $sql = "UPDATE par3.historicosolicitacaoempenho SET ".implode(', ', $coluna)." WHERE hserequest_id = {$hserequest_id} AND hsesistema = {$this->hsesistema} and sisid = {$_SESSION['sisid']}";
            parent::executar($sql);
            parent::commit();
        }
        
        return true;
    }
}
