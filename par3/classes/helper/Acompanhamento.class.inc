<?php

class Par3_Helper_Acompanhamento extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}

	public function recuperarMenusPorCpf($cpf)
	{
		$sql = "SELECT DISTINCT mnu.mnuid, mnudsc, mnulink, mnuicone, mnucor
                FROM seguranca.menu mnu
                INNER JOIN seguranca.perfilmenu pfm ON pfm.mnuid = mnu.mnuid
                INNER JOIN seguranca.perfilusuario pfu ON pfu.pflcod = pfm.pflcod
                LEFT  JOIN par3.menuacompanhamento mna ON mna.mnuid = mnu.mnuid
                WHERE
                	usucpf = '$cpf'
                	AND sisid = 231
                	AND mnulink ilike 'par3.php?modulo=principal/acompanhamento/%'";

		$arrMenus = $this->carregar($sql);

		return $arrMenus;
	}
}