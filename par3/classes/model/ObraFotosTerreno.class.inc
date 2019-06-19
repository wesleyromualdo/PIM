<?php

class Par3_Model_ObraFotosTerreno extends Modelo
{
    protected $stNomeTabela = 'par3.obra_fotos_terreno';

    protected $arChavePrimaria = array(
        'oftid',
    );

    protected $arChaveEstrangeira = array(
        'otfid' => array('tabela' => 'par3.obra_aba_tipo_foto', 'pk' => 'atfid'),
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
    );

    protected $arAtributos = array(
        'oftid' => null,
        'obrid' => null,
        'otfid' => null,
        'oftdsc' => null,
        'oftstatus' => null,
        'arqid' => null,
    );

    public function getCamposValidacao($dados = array())
    {
        return array(
            'oftid' => array('Digits'),
            'obrid' => array('allowEmpty' => true, 'Digits'),
            'otfid' => array('allowEmpty' => true, 'Digits'),
            'oftdsc' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))),
            'oftstatus' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
            'arqid' => array('allowEmpty' => true, 'Digits'),
        );
    }

    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function validarInput(array $campos)
    {
        $erros['erros'] = array();
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value) {
            if (!empty($erros['erros'][$key])) {
                return $erros;
            }
        }
        return false;
    }

    public function montarSQLSimples($arrPost)
    {
        $oftdsc = sanitizar_string_pesquisa($arrPost['oftdsc']);
        $where = $arrPost['oftdsc'] ? " AND upper(removeacento(ofe.oftdsc)) LIKE '%{$oftdsc}%'" : '';
        $where .= $arrPost['otfid'] && validarInteiro($arrPost['otfid']) ? " AND ofe.otfid = {$arrPost['otfid']}" : '';
        $where .= " AND odahinativacao.obrid = {$arrPost['obrid']}";
        $orderBy = $arrPost['ordenacao'] ?
            "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}" :
            " ORDER BY odah.hofdtcriacao ";

        $sql = <<<SQL
SELECT
  ofe.oftid                    AS        id,
  ROW_NUMBER()
  OVER ({$orderBy})           AS        qtd,
  ofe.oftid                    AS        codigo,
  ofe.oftdsc                   AS        descricao,

  arq.arqnome || '.' || arq.arqextensao arquivo,
  odah.hofcpf,
  usu.usunome,
  odah.hofdtcriacao,
  odahinativacao.hofdtcriacao AS        hofdtcriacaoinativacao,
  arq.arqid,
  tpf.atfdsc AS tipo_foto

FROM par3.obra_fotos_terreno ofe
  JOIN par3.obra_fotos_terreno_historico odah ON ((SELECT MIN(hofid)
                                                   FROM par3.obra_fotos_terreno_historico hist
                                                   WHERE hist.oftid = ofe.oftid) = odah.hofid)
  LEFT JOIN par3.obra_fotos_terreno_historico odahinativacao ON ((SELECT MAX(hofid)
                                                                  FROM par3.obra_fotos_terreno_historico hist
                                                                  WHERE hist.oftid = ofe.oftid) =
                                                                 odahinativacao.hofid)
  JOIN seguranca.usuario usu ON (odah.hofcpf = usu.usucpf)
  JOIN public.arquivo arq ON (arq.arqid = ofe.arqid)
  JOIN par3.obra_aba_tipo_foto tpf ON (tpf.atfid = ofe.otfid)
WHERE ofe.oftstatus = 'A'
 {$where}
 {$orderBy}
SQL;
        return $sql;
    }

    public function gerarListaFoto($sql)
    {

        $mpergunta = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
        $disabledAnalise = $mpergunta
            ->habilitarCamposEdicaoPorAbaOrdem(
                Par3_Model_ObraAba::FOTOS_TERRENO,
                1,
                $_GET['obrid']
            );
        //*******************************************

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec(null, null, "par3");

        $dados = $this->carregar($sql);
        $arrFoto = array();
        $html = '';

        if (is_array($dados)) {
            foreach ($dados as $foto) {
                $date = new DateTime($foto['hofdtcriacao']);
                $hashDia = $date->format('dmY');
                $arrFoto[$hashDia][] = $foto;
            }
            $count = 0;
            foreach ($arrFoto as $key => $fotosDia) {
                $dataFormada = new DateTime($arrFoto[$key][$count]['hofdtcriacao']);
                $html .= <<<HTML
        <div class="row">
<h3>Fotos do dia: {$dataFormada->format('d/m/Y')}</h3>
HTML;
                unset($dataFormada);
                foreach ($fotosDia as $foto) {
                    $opcoes = '';
                    if (!$disabledAnalise) {
                        $opcoes = <<<HTML
                 <a class="excluir-foto"  data-oftid="{$foto['id']}"> <i class="fa fa-trash"></i> excluir</a> |
                 <a class="editar-foto"  data-oftid="{$foto['id']}"  > <i class="fa fa-edit"></i> editar</a>
HTML;
                    }
                    unset($disabledAnalise);
                    $path = '/slideshow/slideshow/verimagem.php?newwidth=200&_sisarquivo=par3&arqid='.$foto['arqid'];
                    $html .= <<<HTML
                <div class="col-lg-3">
                <a class="abremodal" data-oftid="{$foto['id']}" data-arqid="{$foto['arqid']}">
                    <img src="{$path}" class="img-responsive img-thumbnail" /></a>
                <div id="descricao-{$foto['arqid']}">
                    <p><b>Tipo:</b> {$foto['tipo_foto']}</p>
                    <p><b>Descrição:</b> {$foto['descricao']}</p>
                    <p>
                </div>
                {$opcoes}
                 </p>
</div>
HTML;
                }
                $html .= <<<HTML
        </div>
HTML;
                $count++;
            }
        }

        echo $html;
    }
}
