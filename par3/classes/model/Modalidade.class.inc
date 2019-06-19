<?php
/**
 * Classe de mapeamento da entidade par3.modalidade.
 *
 * @version $Id$
 * @since 2017.04.17
 */

/**
 * Par3_Model_Modalidade: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Modalidade($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Modalidade($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $modid  - default: nextval('par3.modalidade_modid_seq'::regclass)
 * @property string $moddsc
 * @property "char" $modstatus
 * @property "char" $modsituacao
 * @property \Datetime(Y-m-d H:i:s) $moddtinclusao
 * @property string $modcpfinclusao
 * @property \Datetime(Y-m-d H:i:s) $moddtinativacao
 * @property string $modcpfinativacao
 */
class Par3_Model_Modalidade extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.modalidade';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'modid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array();

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'modid' => null,
        'moddsc' => null,
        'modstatus' => null,
        'modsituacao' => null,
        'moddtinclusao' => null,
        'modcpfinclusao' => null,
        'moddtinativacao' => null,
        'modcpfinativacao' => null,
    );

    public function montarSQLSimples($arrPost)
    {
        $nivid = is_array($arrPost['nivid']) ? implode($arrPost['nivid']) : '';
        $etaid = is_array($arrPost['etaid']) ? implode($arrPost['etaid']) : '';

        $moddsc = sanitizar_string_pesquisa($arrPost['moddsc']);
        $where = $arrPost['moddsc'] ? " AND upper(removeacento(mod.moddsc)) LIKE upper(removeacento('%{$moddsc}%'))" : '';
        $where .= $arrPost['modsituacao'] ? " AND mod.modsituacao = '{$arrPost['modsituacao']}'" : '';
        $where .= $arrPost['modid'] && validarInteiro($arrPost['modid']) ? " AND mod.modid = {$arrPost['modid']}" : '';
        $where .= $arrPost['nivid'] && validarInteiro($arrPost['nivid']) ? " AND niv.nivid in({$arrPost['nivid']})" : '';
        $where .= $arrPost['etaid'] && validarInteiro($arrPost['etaid']) ? " AND eta.etaid in({$arrPost['etaid']})" : '';
        $orderBy = $arrPost['ordenacao'] ? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}" : "";
        $contador = $arrPost['req'] == 'imprimir' || $arrPost['req'] == 'xls' ? "ROW_NUMBER() OVER ({$orderBy}) AS qtd," : '';

        $inicio = $arrPost['req'] == 'imprimir' || $arrPost['req'] == 'xls' ? "SELECT ROW_NUMBER() OVER ({$orderBy}) AS qtd,*FROM (" : '';
        $fim = $arrPost['req'] == 'imprimir' || $arrPost['req'] == 'xls' ? ") as foo" : '';
        $sql = "
            {$inicio}
            SELECT DISTINCT
                mod.modid as id,
                mod.modid as codigo,
                mod.moddsc as descricao,
                (SELECT array_to_string(array(
                    SELECT  DISTINCT
                    niv.nivdsc as nivel
                    FROM par3.ensino_modalidade as ensmod
                    INNER JOIN par3.ensino_etapa as eta ON  ensmod.etaid = eta.etaid
                    INNER JOIN par3.ensino_nivel as niv ON  niv.nivid = eta.nivid
                    WHERE ensmod.modid = mod.modid), ',')
                ) as niveis,
                (SELECT array_to_string(array(
                    SELECT  DISTINCT
                    eta.etadsc as etapa
                    FROM
                    par3.ensino_modalidade as ensmod
                    INNER JOIN par3.ensino_etapa as eta ON  ensmod.etaid = eta.etaid
                    WHERE ensmod.modid = mod.modid), ',')
                ) as etapas,
                mod.modsituacao
            FROM
                par3.modalidade as mod
                LEFT JOIN par3.ensino_modalidade as ensmod ON ensmod.modid = mod.modid
                LEFT JOIN par3.ensino_etapa as eta ON  ensmod.etaid = eta.etaid
                LEFT JOIN par3.ensino_nivel as niv ON  niv.nivid = eta.nivid
                WHERE mod.modstatus = 'A'
                    {$where}
                    {$orderBy}
            {$fim}
            ";
        return $sql;
    }

    public function getModalidade(array $arrPost)
    {
        $where = $arrPost['modid'] ? " AND modid != {$arrPost['modid']}" : '';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE upper(moddsc) = '" . str_to_upper($arrPost['moddsc']) . "' AND modstatus = 'A' {$where}";
        return $this->pegaLinha($sql, 0, 0);
    }

    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['moddsc'] = array();
        $erros['erros']['nivid']  = array();
        $erros['erros']['etaid']  = array();

        //VALIDA CAMPOS
        if (strlen($campos['moddsc']) > 255) {
            array_push($erros['erros']['moddsc'], 'Limite de caracteres excedido');
        }
        if ($campos['moddsc'] == '' || $campos['moddsc'] == null) {
            array_push($erros['erros']['moddsc'], 'O campo não pode ser vazio');
        }

        if (empty(prepararArraySelectMultiSelecao($campos['nivid']))) {
            array_push($erros['erros']['nivid'], 'O campo não pode ser vazio');
        }
        if (empty(prepararArraySelectMultiSelecao($campos['etaid']))) {
            array_push($erros['erros']['etaid'], 'O campo não pode ser vazio');
        }

        if ($this->getModalidade($campos)) {
            array_push($erros['erros']['moddsc'], 'A descrição informada já existe');
        }

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value) {
            if (!empty($erros['erros'][$key])) {
                return $erros;
            }
        }
        return false;
    }

    /**
     * Lista de Modalidades.
     *
     * @return array|mixed|NULL
     */
    public function listaModalidade($etaid)
    {
        return $this->carregar(
            "SELECT DISTINCT
                mod.modid as codigo,
                mod.moddsc as descricao
            FROM par3.modalidade as mod
            INNER JOIN par3.ensino_modalidade as ensmod
              ON ensmod.modid = mod.modid
            INNER JOIN par3.ensino_etapa as eta
              ON  ensmod.etaid = eta.etaid
            INNER JOIN par3.ensino_nivel as niv
              ON  niv.nivid = eta.nivid
            WHERE mod.modstatus = 'A' AND
                  mod.modsituacao = 'A' AND
                  eta.etaid = $etaid"
        );

    }

    public function verificaLigacaoPlanejamento($modid){
        $sql = "SELECT * FROM 
         par3.iniciativa_planejamento as iniciativa_planejamento
         INNER JOIN par3.iniciativa as iniciativa ON iniciativa.iniid = iniciativa_planejamento.iniid
         INNER JOIN par3.iniciativa_modalidades as iniciativa_modalidades ON iniciativa_modalidades.iniid = iniciativa.iniid
         INNER JOIN par3.modalidade as modalidade ON modalidade.modid = iniciativa_modalidades.modid 
	     WHERE modalidade.modid = {$modid}";

        $result = $this->carregar($sql);
        return $result;
    }

    public function getModalidadeById($modid){
        $sql = "SELECT * FROM par3.modalidade WHERE modid = {$modid}";
        $result = $this->pegaLinha($sql);
        return $result;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param null $modid
     * @param null $etaid
     * @param null $nivid
     * @return string
     * @description Esta função retorna o sql que monta o simec->select(), caso exista alguns dos parametros (modid|etaid| nivid) monta sql com WHERE
     */
    public function getFormListModalidade($modid = null, $etaid = null, $nivid = null){

        $where = '';
        $WHRE_EXISTS = 0;
       if (!is_null($modid) || !is_null($etaid) || !is_null($nivid)){
            if(!is_null($modid)){
                    $where .= " modid = {$modid} ";
                    $WHRE_EXISTS++;
            }
            if(!is_null($etaid)){
                if($WHRE_EXISTS > 0){
                    $where .= " AND ";
                }
                $where .= " modid = {$modid} ";
                $WHRE_EXISTS++;
            }
            if(!is_null($nivid)){
                if($WHRE_EXISTS > 0){
                    $where .= " AND ";
                }
                $where .= " nivid = {$nivid} ";
                $WHRE_EXISTS++;
            }
        }
        $sql =  '';
        if($WHRE_EXISTS>0){
            $sql = "select modid as codigo, moddsc as descricao from par3.modalidade order by moddsc asc;";
        }else{
            $sql = "select modid as codigo, moddsc as descricao from par3.modalidade {$where} order by moddsc asc;";
        }

        return $sql;
    }

}
