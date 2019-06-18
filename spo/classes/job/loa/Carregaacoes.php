<?php
/**
 * Job para carregar a tabela monitora.acao.
 *
 * CUIDADO: Este job apaga a tabela monitora.acao para o exercicio selecionado.
 *
 * @version $Id: Carregaacoes.php 136944 2018-01-25 22:35:33Z maykelbraz $
 */

class Spo_Job_Loa_Carregaacoes extends Simec_Job_Abstract
{
    protected $params = [];

    public function getName()
    {
        return 'Carregando ações e localizadores...';
    }

    protected function init()
    {
        $this->params = array_merge(
            $this->params,
            $this->loadParams()
        );
    }

    protected function execute()
    {
        echo "Apagando ações e localizadores do exercício {$this->params['exercicio']}...<br />";
        $this->salvarOutput();
        $this->apagarAcoes($this->params['exercicio']);

        echo 'Carregando novas ações e localizadores...<br />';
        $this->salvarOutput();
        $qtdAcoesInseridas = $this->inserirAcoes(
            $this->params['exercicio'],
            $this->params['momento']
        );

        echo "{$qtdAcoesInseridas} ações e localizadores inseridas...";
        $this->salvarOutput();
    }

    protected function shutdown()
    {
    }

    protected function apagarAcoes($exercicio)
    {
        $this->setTable('monitora.acao');
        $this->jobDelete(["prgano = '{$exercicio}' AND acacod <> '20RG'"]);
    }

    protected function inserirAcoes($exercicio, $momento)
    {
        // -- Ajustando sequence para tratar exceção do TED - 20RG
        $sql = <<<DML
SELECT setval('monitora.acao_acaid_seq', COALESCE((SELECT MAX(acaid)+1 FROM monitora.acao), 1), false);
DML;
        $this->executar($sql);

        $sql = <<<DML
INSERT INTO monitora.acao (acacod,
                           saccod,
                           loccod,
                           esfcod,
                           unicod,
                           unitpocod,
                           funcod,
                           regcod,
                           taccod,
                           procod,
                           prgano,
                           prgcod,
                           sitcodestagio,
                           sitcodandamento,
                           sitcodcronograma,
                           acadsc,
                           sacdsc,
                           acasnmetanaocumulativa,
                           acadscsituacaoatual,
                           acasnrap,
                           acadescricao,
                           acabaselegal,
                           acasntransfoutras,
                           acadetalhamento,
                           sfucod,
                           acastatus,
                           acasnestrategica,
                           acasnbgu,
                           acaptres,
                           acadataatualizacao,
                           acatipoinclusao,
                           acatipoinclusaolocalizador,
                           unmdsc,
                           prodsc,
                           descricaomomento,
                           unidaderesponsavel,
                           acatitulo,
                           ididentificadorunicosiop,
                           unmcodsof,
                           procodsof,
                           acatipocod,
                           acatipodsc,
                           acainiciativacod,
                           acainiciativadsc,
                           acaobjetivocod,
                           acaobjetivodsc,
                           prgdsc,
                           esfdsc,
                           fundsc,
                           sfundsc)
SELECT aca.codigoacao AS acacod,
       lpad(loc.codigolocalizador, 4, '0') AS saccod,
       lpad(loc.codigolocalizador, 4, '0') AS loccod,
       aca.codigoesfera::INTEGER AS esfcod,
       aca.codigoorgao AS unicod,
       'U' AS unitpocod,
       aca.codigofuncao AS funcod,
       CASE WHEN TRIM(loc.uf) <> '' THEN UPPER(TRIM(loc.uf))
            ELSE NULL
         END AS regcod,
       1 AS taccod,
       CASE WHEN aca.codigoproduto = '' THEN NULL
            ELSE aca.codigoproduto::INTEGER
         END AS procod,
      '{$exercicio}' AS prgano,
      aca.codigoprograma AS prgcod,
      '00' AS sitcodestagio,
      '00' AS sitcodandamento,
      '00' AS sitcodcronograma,
	  aca.titulo AS acadsc,
	  loc.descricao AS sacdsc,
      TRUE AS acasnmetanaocumulativa,
      FALSE AS acadscsituacaoatual,
      FALSE AS acasnrap,
      aca.descricao AS acadescricao,
      aca.baselegal AS acabaselegal,
      FALSE AS acasntransfoutras,
      aca.detalhamentoimplementacao AS acadetalhamento,
      aca.codigosubfuncao AS sfucod,
      'A' AS acastatus,
      FALSE AS acasnestrategica,
      FALSE AS acasnbgu,
      '@@@' AS acaptres,
      CURRENT_DATE AS acadataatualizacao,
      aca.codigotipoinclusaoacao AS acatipoinclusao,
      loc.codigotipoinclusao AS acatipoinclusaolocalizador,
      unm.descricao AS unmdsc,
      pro.descricao AS prodsc,
      '{$momento}' AS descricaomomento,
      aca.unidaderesponsavel AS unidaderesponsavel,
      aca.titulo AS acatitulo,
      aca.identificadorunico AS ididentificadorunicosiop,
      aca.codigounidademedida AS unmcodsof,
      pro.codigoproduto AS procodsof,
      aca.codigotipoacao AS acatipocod,
      tac.descricao AS acatipodsc,
      aca.codigoiniciativa AS acainiciativacod,
      ini.titulo AS acainiciativadsc,
      aca.codigoobjetivo AS acaobjetivocod,
      obj.enunciado AS acaobjetivodsc,
      prg.titulo AS prgdsc,
      esf.descricao AS esfdsc,
      fun.descricao AS fundsc,
      sfun.descricao AS sfundsc
  FROM spo.job_loa_acoesdto aca
    JOIN spo.job_loa_localizadoresdto loc ON aca.identificadorunico = loc.identificadorunicoacao
    LEFT JOIN spo.job_loa_produtosdto pro ON aca.codigoproduto::VARCHAR = pro.codigoproduto::VARCHAR
    LEFT JOIN spo.job_loa_unidadesmedidadto unm ON aca.codigounidademedida = unm.codigounidademedida
    JOIN spo.job_loa_tiposacaodto tac ON aca.codigotipoacao = tac.codigotipoacao
    LEFT JOIN spo.job_loa_iniciativasdto ini ON aca.codigoiniciativa = ini.codigoiniciativa
    LEFT JOIN spo.job_loa_objetivosdto obj ON aca.codigoobjetivo = obj.codigoobjetivo
    LEFT JOIN spo.job_loa_programasdto prg ON prg.codigoprograma = aca.codigoprograma
    LEFT JOIN spo.job_loa_esferasdto esf ON esf.codigoesfera = aca.codigoesfera
    LEFT JOIN spo.job_loa_funcoesdto fun ON fun.codigofuncao = aca.codigofuncao
    LEFT JOIN spo.job_loa_subfuncoesdto sfun ON sfun.codigosubfuncao = aca.codigosubfuncao
  WHERE aca.codigoacao <> '20RG'
DML;

        $this->executar($sql);
        if (!($this->commit())) {
            throw new Exception('Não foi possível inserir as ações.');
        }

        $sql = <<<DML
SELECT COUNT(1)
  FROM monitora.acao
  WHERE prgano = '{$exercicio}'
DML;
        return $this->pegaUm($sql);
    }
}
