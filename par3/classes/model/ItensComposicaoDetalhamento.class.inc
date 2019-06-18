<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao.
 *
 * @version $Id$
 * @since 2017.06.09
 */

/**
 * Model_Itenscomposicao: sem descricao
 *
 * @package Model\Itenscomposicao
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Itenscomposicao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Itenscomposicao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $itcid Chave Primária - default: nextval('par3.itenscomposicao_itcid_seq'::regclass)
 * @property int $ctdid Chave estrangeira par3.categoriadespesa - default: nextval('par3.itenscomposicao_ctdid_seq'::regclass)
 * @property int $uniid Chave estrangeira par3.unidade_medida - default: nextval('par3.itenscomposicao_uniid_seq'::regclass)
 * @property string $itcdsc
 * @property string $itcespecificacao Especificação Técnica
 * @property "char" $itcstatus Status Item de Composição (Soft Delete)
 * @property "char" $itcsituacao Situação Item de Composição
 * @property \Datetime(Y-m-d H:i:s) $itcdtinclusao Data de inclusão do Item de Composição
 * @property string $itccpfinclusao CPF do responsável pela inclusão de um Item de Composição
 * @property \Datetime(Y-m-d H:i:s) $itcdtinativacao Data de Inativação de um Item de Composição
 * @property string $itccpfinativacao
 */
class Par3_Model_ItenscomposicaoDetalhamento extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_detalhamento';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'itdid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'itcid' => array('tabela' => 'par3.itenscomposicao',           'pk' => 'itcid'),
        'sprid' => array('tabela' => 'par.sigarppregao',               'pk' => 'sprid'),
        'atpid' => array('tabela' => 'par.itenscomposicao_ata_pregao', 'pk' => 'atpid'),

    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'itdid'              => null,
        'itcid'              => null,
        'sprid'              => null,
        'atpid'              => null,
        'itdpregao'          => null,
        'itdpregaopublicado' => null,
        'itdvalor'           => null,
        'itdperiodoinicio'   => null,
        'itdperiodofim'      => null,
        'itdstatus'          => null,
        'itdsituacao'        => null,
        'itddtinclusao'      => null,
        'itdcpfinclusao'     => null,
        'itddtinativacao'    => null,
        'itdcpfinativacao'   => null,

    );

    public function getItemDetalhamentoDesc($arrPost)
    {
//        $where = $arrPost['itdid']?" AND itdid != {$arrPost['itdid']}":'';
//        return $this->pegaUm("SELECT * FROM {$this->stNomeTabela} WHERE LOWER(itdid) = LOWER('{$arrPost['itdid']}') AND itdstatus = 'A' {$where}");
    }

    public function validarInput($campos)
    {
        //campos
        $erros['erros']['itcid']              = array();//par3.itenscomposicao
        $erros['erros']['sprid']              = array();//par.sigarppregao
        $erros['erros']['atpid']              = array();//par3.itenscomposicao_ata_pregao
        $erros['erros']['itdpregao']          = array();//define se é pregão
        $erros['erros']['itdpregaopublicado'] = array();//define se é pregão publicado
        $erros['erros']['itdvalor']           = array();//valor item detalhamento
        $erros['erros']['itdperiodoinicio']   = array();//data período início
        $erros['erros']['itdperiodofim']      = array();//data período fim
        $erros['erros']['itdsituacao']        = array();//situação
        $erros['erros']['itdperiodo']         = array();
        $erros['erros']['estuf']              = array();

        $itd = new $this($campos['itdid']);
        if(empty($campos['itdpregao']) || $campos['itdpregao'] == null){array_push($erros['erros']['itdpregao'],'Selecione uma opção.');}

        //VALIDA SE FOR PREGÃO
        if($campos['itdpregao'] == 'S'){
            if(empty($campos['itdpregaopublicado']) || $campos['itdpregaopublicado'] == null){array_push($erros['erros']['itdpregaopublicado'],'Selecione uma opção.');}
        }
        //ver($campos,d);
        //SE A AÇÃO FOR DE CRIAÇÃO... VERIFICA SE EXISTE DETALHAMENTO COM O PERIODO E ESTADO INFORMADO
        if($campos['acao'] == "CREATE"){
            $consultaSeExisteDetalhamento = $this->consultaExistenciaDetalhamento($campos);
             if($consultaSeExisteDetalhamento == 1) { array_push($erros['erros']['itdperiodo'],'Já existe detalhamento de composição com esse período e estado.'); }
        }

        if($campos['itdpregao'] == 'S' && $campos['itdpregaopublicado'] == 'S'){
            if(empty($campos['sprid'])  || $campos['sprid'] == null){array_push($erros['erros']['sprid'],'Selecione uma opção.');}
            if(empty($campos['atpid'])  || $campos['atpid'] == null){array_push($erros['erros']['atpid'],'Selecione uma opção.');}
            if(!validarInteiro($campos['atpid'])){array_push($erros['erros']['atpid'],'Opção inválida.');}
            if(!validarInteiro($campos['sprid'])){array_push($erros['erros']['sprid'],'Opção inválida.');}
        }
        //VALIDAR VALOR
        if(!is_numeric($campos['itdvalor'])){array_push($erros['erros']['itdvalor'],'Valor inválido');}
        if(empty(prepararArraySelectMultiSelecao($campos['estuf']))){array_push($erros['erros']['estuf'],'Selecione pelo menos um Estado.');}

        //VALIDAR PERÍODOS
        if($campos['itdperiodoinicio'] == '' && !$itd->itdperiodoinicio ){array_push($erros['erros']['itdperiodo'],'O período de início não pode ser vazio');}
        if($campos['itdperiodofim'] == '' && !$itd->itdperiodofim){array_push($erros['erros']['itdperiodo'],'O fim do período não pode ser vazio');}
        if($campos['itdperiodoinicio'] > $campos['itdperiodofim']){array_push($erros['erros']['itdperiodo'],'O início do período não pode ser maior do que o fim do período');}
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    public function montarSQLDataGrid($arrPost)
    {
        $itcid = $arrPost['itcid'];
        /**
         * ordenação
         * id =1,codigo = 2,detalhamento = 3,atapregao = 4,estados = 5,periodoinicio = 6,periodofim = 7,situacao = 8
         */
        $sql = "
                SELECT DISTINCT
                itd.itdid  as id,
                itd.itdid  as codigo,
                ata.atpnumeroata as atapregao,
                (SELECT array_to_string(array(
                    SELECT  DISTINCT
                    estuf.estdescricao
                    FROM par3.itenscomposicao_detalhamento_estado as ide
                    INNER JOIN territorios.estado as estuf ON estuf.estuf = ide.estuf
                    WHERE ide.itdid = itd.itdid ORDER BY estuf.estdescricao ASC), ', ')
                ) as estados,
                'R$'||formataMonetario(itd.itdvalor) as valor,
                itd.itdperiodoinicio as periodoinicio,
                itd.itdperiodofim as periodofim,
                itd.itdsituacao as situacao
                FROM {$this->stNomeTabela} as itd
                LEFT JOIN par3.itenscomposicao_ata_pregao as ata ON ata.atpid = itd.atpid
                WHERE itd.itdstatus = 'A'
                AND itcid = {$itcid}
                ";
        return $sql;
    }

    public function consultaExistenciaDetalhamento($arrPost){

        $estados = implode("','",$arrPost['estuf']);
        
        $sql = "  SELECT itd.itdid , to_char(itd.itdperiodoinicio, 'YYYY-MM-DD'), to_char(itd.itdperiodofim, 'YYYY-MM-DD'), estuf.estdescricao
                  FROM par3.itenscomposicao_detalhamento itd
                  INNER JOIN par3.itenscomposicao_detalhamento_estado as ide 
                  ON ide.itdid = itd.itdid
                  INNER JOIN territorios.estado estuf 
                  ON estuf.estuf = ide.estuf 
                  WHERE itd.itdstatus = 'A' 
                  AND itd.itcid = {$arrPost['itcid']}
                  AND to_char(itd.itdperiodoinicio, 'YYYY-MM-DD') = '{$arrPost['itdperiodoinicio']}'
                  AND to_char(itd.itdperiodofim, 'YYYY-MM-DD') = '{$arrPost['itdperiodofim']}'
                  AND estuf.estuf in ('{$estados}')";

        return $this->pegaUm($sql);
    }
}
