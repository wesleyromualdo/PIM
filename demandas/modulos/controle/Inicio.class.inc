<?php
require APPRAIZ . 'demandas/model/Demandantes.class.inc';

class Inicio extends mvc\Controle
{
	public function iniciarFluxo()
	{
        $this->visao->setCabecalho('includes/cabecalho_bootstrap_v4.inc', true);
        $this->visao->setRodape('includes/rodape_bootstrap_v4.inc', true);
	}
	
	public function acaoPainel()
	{    
	    $this->visao->incluirVisao('painel');
	}
}