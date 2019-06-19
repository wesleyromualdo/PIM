<?php
/**
 * Created by PhpStorm.
 * User: victor
 * Date: 17/01/18
 * Time: 19:56
 */

require_once APPRAIZ . 'monitora/classes/Pi_Subacao.class.inc';

class Spo_Job_Loa_Carregasubacoes extends Simec_Job_Abstract
{
    protected $params = [
        'tabela' => 'monitora.pi_subacao',
    ];
    protected $anoAtual;
    protected $anoAnterior;

    public  function getName()
    {
        return 'Carregando subações...';
    }

    protected function init()
    {
        $this->anoAtual = $this->params['exercicio'];
        $this->anoAnterior = $this->anoAtual - 1;

        $this->setTable($this->params['tabela']);

        $this->params = array_merge(
            $this->params,
            $this->loadParams()
        );
        return true;
    }

    protected function execute()
    {
        echo "Carregando novas subações...<br />";
        $this->salvarOutput();
        $this->inserirSubacoes();
    }

    protected  function shutdown()
    {
        // TODO: Implement shutdown() method.
    }

    protected function inserirSubacoes()
    {
        global $db;

        $insert = <<<SQL
INSERT INTO monitora.pi_subacao (sbadsc,
                                 sbastatus,
                                 sbadata,
                                 sbasigla,
                                 sbatitulo,
                                 sbacod,
                                 usucpf,
                                 sbasituacao,
                                 sbaobras,
                                 sbaplanotrabalho,
                                 pieid,
                                 pigid,
                                 sbaano)
SELECT sba.sbadsc,
       sba.sbastatus,
       now(),
       sba.sbasigla,
       sba.sbatitulo,
       sba.sbacod,
       sba.usucpf,
       sba.sbasituacao,
       sba.sbaobras,
       sba.sbaplanotrabalho,
       sba.pieid,
       sba.pigid,
       '{$this->anoAtual}'
  FROM monitora.pi_subacao sba
    JOIN monitora.pi_subacaounidade psu ON (psu.sbaid = sba.sbaid)
    WHERE sba.sbaano ='{$this->anoAnterior}';
SQL;
        $cont = pg_affected_rows($db->executar($insert));

        echo "{$cont} Subações carregadas...<br />";
        $this->salvarOutput();
        return;
    }
}
