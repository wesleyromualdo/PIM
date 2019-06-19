<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao_ata_pregao.
 *
 * @version $Id$
 * @since 2017.06.05
 */

/**
 * Model_Itenscomposicao_ata_pregao: sem descricao
 *
 * @package Model\Itenscomposicao
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Itenscomposicao_ata_pregao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Itenscomposicao_ata_pregao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $atpid Chave Primária - default: nextval('par3.itenscomposicao_ata_pregao_atpid_seq'::regclass)
 * @property string $atpnumeroata Número da Ata de Pregão
 * @property string $atpanovigencia
 * @property \Datetime(Y-m-d H:i:s) $atpdtinicio Data de início da Ata de Pregão
 * @property \Datetime(Y-m-d H:i:s) $atpdtfim Data de término da Ata de Pregão
 * @property string $atpobjeto Descrição do Objeto da Ata de Pregão
 * @property int $arqid Código do arquivo da Ata de Pregão
 * @property string $atpdescricaoanexo Descrição do arquivo da Ata de Pregão
 * @property "char" $atpstatus status da Ata de Pregão (soft delete)
 * @property "char" $atpsituacao situação da Ata de Pregão, indica se está ativo/inativo
 * @property \Datetime(Y-m-d H:i:s) $atpdtinclusao data de inclusão da Ata de Pregão
 * @property string $atpcpfinclusao cpf do responsável pela inclusao da Ata de Pregão
 * @property \Datetime(Y-m-d H:i:s) $atpdtinativacao data de inativação da Ata de Pregão
 * @property int $atpvigencia
 * @property string $atpcpfinativacao cpf do responsável pela inativação da Ata de Pregão
 */
class Par3_Model_ItensComposicaoAtaPregao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_ata_pregao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'atpid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'atpid' => null,
        'atpnumeroata' => null,
        'atpanovigencia' => null,
        'atpdtinicio' => null,
        'atpdtfim' => null,
        'atpobjeto' => null,
        'arqid' => null,
        'atpdescricaoanexo' => null,
        'atpstatus' => null,
        'atpsituacao' => null,
        'atpdtinclusao' => null,
        'atpcpfinclusao' => null,
        'atpdtinativacao' => null,
        'atpvigencia' => null,
        'atpcpfinativacao' => null,
    );

    public function getAta($arrPost)
    {
        $where = $arrPost['atpid']?" AND atpid != {$arrPost['atpid']}":'';
        return $this->pegaUm("SELECT * FROM {$this->stNomeTabela} WHERE LOWER(atpnumeroata) = LOWER('{$arrPost['atpnumeroata']}') AND atpstatus = 'A' {$where}");
    }

    public function validarVigenciaAta($atpid,$dataFimNova)
    {
        $atp = new $this($atpid);
        $hoje = date('Y-m-d H:i:s');
        $dataFimAntiga    = date('Y-m-d H:i:s', strtotime($atp->atpdtfim));//Data Fim de uma ata existente
        $dataFimNova      = date('Y-m-d H:i:s',strtotime($dataFimNova));//Nova Data Fim
        if($dataFimAntiga == $dataFimNova || !empty($dataFimAntiga)){return true;}
        if($dataFimAntiga < $hoje){return false;}
        return true;
    }

    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['atpnumeroata']       = array();
        $erros['erros']['atpanovigencia']     = array();
        $erros['erros']['atpdtinicio']        = array();
        $erros['erros']['atpdtfim']           = array();
        $erros['erros']['atpobjeto']          = array();
        $erros['erros']['arqid']              = array();
        $erros['erros']['atpdescricaoanexo']  = array();
        $erros['erros']['atpvigencia']        = array();
        $erros['erros']['atpperiodo']         = array();
        $erros['erros']['atpvigencia']        = array();
        $erros['erros']['estuf']              = array();

        $atp = new $this($campos['atpid']);
        if($atp->atpid){
            if($this->validarVigenciaAta($campos['atpid'],$campos['atpdtfim']) == false){
                array_push($erros['erros']['atpperiodo'],'A data fim do período não pode ser alterada fora do período de vigência');
            }
        }
        if(empty(prepararArraySelectMultiSelecao($campos['estuf']))){array_push($erros['erros']['estuf'],'Selecione uma UF');}
        if($campos['atpvigencia'] == '' && !$atp->atpvigencia){array_push($erros['erros']['atpvigencia'],'O campo vigência não pode ser vazio');}
        if($campos['atpdtinicio'] == '' && !$atp->atpdtinicio ){array_push($erros['erros']['atpperiodo'],'O período de início não pode ser vazio');}
        if($campos['atpdtfim'] == '' && !$atp->atpdtfim){array_push($erros['erros']['atpperiodo'],'O fim do período não pode ser vazio');}
        if($campos['atpdtinicio'] > $campos['atpdtfim']){array_push($erros['erros']['atpperiodo'],'O início do período não pode ser maior do que o fim do período');}
        if($campos['atpanovigencia'] == '' || $campos['atpanovigencia'] == null){array_push($erros['erros']['atpanovigencia'],'O campo não pode ser vazio');}
        //VALIDA CAMPOS
        if(strlen($campos['atpnumeroata']) > 255){array_push($erros['erros']['atpnumeroata'],'Limite de caracteres excedido');}
        if($campos['atpnumeroata'] == '' || $campos['atpnumeroata'] == null){array_push($erros['erros']['atpnumeroata'],'O campo não pode ser vazio');}
        if($this->getAta($campos)){array_push($erros['erros']['atpnumeroata'],'Já Existe Ata Cadastrada com esse valor.');}

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    public function montarSQLSimples($arrPost)
    {
        $dataVigencia = $arrPost['vigencia']?formata_data_sql($arrPost['vigencia']):"";
        $atpobjeto    = sanitizar_string_pesquisa($arrPost['atpobjeto']);
        $atpnumeroata = sanitizar_string_pesquisa($arrPost['atpnumeroata']);

        $where   = $arrPost['atpobjeto']?" AND upper(removeacento(atpobjeto)) LIKE '%{$atpobjeto}%' ":'';
        $where  .= $arrPost['atpnumeroata']?" AND upper(removeacento(atp.atpnumeroata)) LIKE '%{$atpnumeroata}%' ":'';
        $where  .= $arrPost['atpsituacao']?" AND atp.atpsituacao = '{$arrPost['atpsituacao']}' ":'';
        $where  .= $dataVigencia?" AND '{$dataVigencia}' BETWEEN atp.atpdtinicio AND atp.atpdtfim":"";
        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";
        /**
         * ordenação
         * id =1,qtd = 2,codigo = 3,numerodata = 4,atpobjeto = 5,ano = 6,arquivo,7,descricaoanexo = 8,estados = 9,vigencia = 11,situacao = 12
         */
        $sql = "SELECT
                atp.atpid as id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                atp.atpid as codigo,
                atp.atpnumeroata as numeroata,
                atp.atpobjeto,
                atp.atpanovigencia as ano,
                arq.arqnome||'.'||arq.arqextensao as arquivo,
                atp.atpdescricaoanexo as descricaoanexo,
                (SELECT array_to_string(array(
                    SELECT  DISTINCT
                    est.estuf
                    FROM par3.itenscomposicao_ata_pregao_estado as ate
                    INNER JOIN territorios.estado as est ON est.estuf = ate.estuf
                    WHERE ate.atpid = atp.atpid ORDER BY est.estuf ASC), ', ')
                ) as estados,
                to_char(atp.atpdtinicio,'DD/MM/YYYY')||' a '||to_char(atp.atpdtfim,'DD/MM/YYYY') as vigencia,
                atp.atpsituacao as situacao
                FROM {$this->stNomeTabela} as atp
                LEFT JOIN public.arquivo as arq ON arq.arqid = atp.arqid
                WHERE atpstatus = 'A'
                {$where}
                {$orderBy}
                ";
        return $sql;
    }

}
